#include "UI/Widget/Inventory.h"
#include "Player/EntityPlayer.h"
#include "Components/Inventory/BackpackComponent.h"
#include "UI/Widget/StorageUI/InventoryItem.h"
#include "Components/UniformGridPanel.h"
#include "UI/Widget/DroppableInventoryCellWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

#include "Item/Crafting/CraftingManager.h"
#include "Item/Crafting/IRecipe.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
#include "Interfaces/InventoryInterface.h"

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();

	Player = Player == nullptr ? GetOwningPlayerPawn<AEntityPlayer>() : Player;

	if (Player)
	{
		BackpackComponent = Player->GetComponentByClass<UBackpackComponent>();
	}

	if (BackpackComponent)
	{
		BackpackComponent->OnHotbarUpdate.AddDynamic(this, &UInventory::FlushHotbar);
		BackpackComponent->OnInventoryUpdate.AddDynamic(this, &UInventory::FlushInventory);
	}
}

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	InitHotbarWidget();
	InitInventoryWidget();

	InitUI();
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(DroppableItem))
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DroppableItem))
		{
			double LocationX = 0.0;
			double LocationY = 0.0;
			UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), LocationX, LocationY);

			FVector2D Location2D(LocationX, LocationY);
			CanvasPanelSlot->SetPosition(Location2D - HangImageSize / 2);
		}
	}
}

void UInventory::InitUI()
{
	FlushHotbar();
	FlushInventory();
}

void UInventory::FlushHotbar()
{
	if (!Hotbar) return;

	for (int32 i = 0; i < 9; ++i)
	{
		UInventoryItem* InventoryItem = Cast<UInventoryItem>(Hotbar->GetChildAt(i));
		if (InventoryItem)
		{
			InventoryItem->FlushItemData();
		}
	}
}

void UInventory::FlushInventory()
{
	if (!Inventory) return;

	for (int32 i = 9; i < BackpackComponent->GetSizeInventory_Implementation(); ++i)
	{
		UInventoryItem* InventoryItem = Cast<UInventoryItem>(Inventory->GetChildAt(i - 9));
		if (InventoryItem)
		{
			InventoryItem->FlushItemData();
		}
	}
}

void UInventory::InitHotbarWidget()
{
	if (!Hotbar || !InventoryItemClass) return;

	for (int32 i = 0; i < 9; ++i)
	{
		UInventoryItem* InventoryItem = CreateWidget<UInventoryItem>(this, InventoryItemClass);
		if (InventoryItem)
		{
			InventoryItem->Inventory = BackpackComponent;
			InventoryItem->Parent = this;
			InventoryItem->Index = i;

			Hotbar->AddChildToUniformGrid(InventoryItem, 0, i);
		}
	}
}

void UInventory::InitInventoryWidget()
{
	if (!Inventory || !InventoryItemClass) return;

	for (int32 i = 9; i < BackpackComponent->GetSizeInventory_Implementation(); ++i)
	{
		UInventoryItem* InventoryItem = CreateWidget<UInventoryItem>(this, InventoryItemClass);
		if (InventoryItem)
		{
			InventoryItem->Inventory = BackpackComponent;
			InventoryItem->Parent = this;
			InventoryItem->Index = i;

			Inventory->AddChildToUniformGrid(InventoryItem, (i - 9) / 9, (i - 9) % 9);
		}
	}
}

void UInventory::HandleLMB_Implementation(UInventoryItem* InventoryItem)
{
	if (!bIsDragInProgress)
	{
		if (!InventoryItem->GetItemStack().IsEmpty())
		{
			DragFromCellIndex = InventoryItem->Index;
			HangItemStack = IInventoryInterface::Execute_RemoveStackFromSlot(InventoryItem->Inventory.GetObject(), InventoryItem->Index);
			CreateDroppableItemWidget(HangItemStack);

			OnHandleLMB(InventoryItem); 
		}
	}
	else
	{
		if (IInventoryInterface::Execute_AddItemToInventoryFromIndex(InventoryItem->Inventory.GetObject(), InventoryItem->Index, HangItemStack))
		{
			
		}
		else
		{

		}
	}

	UpdateDroppableItemWidget();
}

void UInventory::RemoveDroppableItemWidget()
{
	if (bIsDragInProgress && IsValid(DroppableItem))
	{
		DroppableItem->RemoveFromParent();
		DroppableItem = nullptr;
		bIsDragInProgress = false;
	}
}

void UInventory::HandleRMB_Implementation(UInventoryItem* InventoryItem)
{
	if (bIsDragInProgress)
	{
		FItemStack ClickedItemStack = InventoryItem->GetItemStack();

		if (HangItemStack.GetItem() == ClickedItemStack.GetItem())
		{
			HangItemStack.Shrink(1);
			InventoryItem->Grow(1);
		}
		else
		{
			FItemStack ItemStack = InventoryItem->GetItemStack();
			if (ItemStack.IsEmpty())
			{
				InventoryItem->SetInventorySlotContents(HangItemStack.SplitStack(1));
			}
			else
			{
				InventoryItem->SetInventorySlotContents(HangItemStack);
				HangItemStack = ItemStack;
			}
			
		}
	}
	else
	{
		if (InventoryItem->IsEmpty()) return;

		HangItemStack = InventoryItem->DecrStackSize(1);
		CreateDroppableItemWidget(HangItemStack);
	}

	UpdateDroppableItemWidget();
}

void UInventory::OnHandleLMB(UInventoryItem* InventoryItem)
{
}

void UInventory::OnCraftMatrixChanged()
{

}

void UInventory::SlotChangedCraftingGrid(AEntityPlayer* PlayerIn, UCraftingComponent* CraftingMatrix, UCraftingResultComponent* CraftingResult)
{
	FItemStack ItemStack;

	TSharedPtr<IRecipe> Recipe = CraftingManager::FindMatchingRecipe(CraftingMatrix);
	if (Recipe.IsValid())
	{
		CraftingResult->SetRecipeUsed(Recipe);
		ItemStack = Recipe->GetCraftingResult();
	}

	CraftingResult->SetInventorySlotContents_Implementation(0, ItemStack);
}

void UInventory::CreateDroppableItemWidget(const FItemStack& ItemStack)
{
	if (UDroppableInventoryCellWidget* DroppableInventoryCellWidget = CreateWidget<UDroppableInventoryCellWidget>(this, DroppableInventoryCellWidgetClass))
	{
		DroppableInventoryCellWidget->FlushItemData(ItemStack);
		UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel->AddChildToCanvas(DroppableInventoryCellWidget);
		CanvasPanelSlot->SetSize(HangImageSize);
		DroppableInventoryCellWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		DroppableItem = DroppableInventoryCellWidget;
		bIsDragInProgress = true;
	}
}

void UInventory::UpdateDroppableItemWidget()
{
	if (bIsDragInProgress && IsValid(DroppableItem))
	{
		DroppableItem->FlushItemData(HangItemStack);
		if (HangItemStack.IsEmpty())
		{
			RemoveDroppableItemWidget();
		}
	}
}
