// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Container.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/Widget/DroppableInventoryCellWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Inventory/BackpackComponent.h"
#include "Components/UniformGridPanel.h"
#include "MinecraftGameplayTags.h"
#include "Components/CanvasPanel.h"

UContainer::UContainer()
{
	static ConstructorHelpers::FClassFinder<USlot> InventorySlotClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Widget/WBP_Slot.WBP_Slot_C'"));
	if (InventorySlotClassFinder.Succeeded())
	{
		SlotClass = InventorySlotClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UDroppableInventoryCellWidget> HangItemClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Widget/WBP_DroppableInventoryCellWidget.WBP_DroppableInventoryCellWidget_C'"));
	if (HangItemClassFinder.Succeeded())
	{
		HangItemClass = HangItemClassFinder.Class;
	}
}

void UContainer::SetActor(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
}

void UContainer::NativeConstruct()
{
	SetIsFocusable(true);
	SetInputModeUIOnly();

	BackpackComp = GetOwningPlayerPawn()->GetComponentByClass<UBackpackComponent>();

	BindCallbacksToDependencies();

	InitBackpack();

	Super::NativeConstruct();
}

void UContainer::BindCallbacksToDependencies()
{
	BackpackComp->OnInventoryItemUpdateDelegate.AddDynamic(this, &UContainer::OnBagItemUpdateDelegateEvent);
	BackpackComp->OnHotbarItemUpdateDelegate.AddDynamic(this, &UContainer::OnHotbarItemUpdateDelegateEvent);
	BackpackComp->OnHangItemUpdateDelegate.AddDynamic(this, &UContainer::OnHangItemUpdateEvent);
}

void UContainer::InitBackpack()
{
	ensure(SlotClass);

	Hotbar->ClearChildren();
	for (int32 i = 0; i < 9; ++i)
	{
		USlot* InventorySlot = CreateWidget<USlot>(this, SlotClass);
		InventorySlot->Parent = this;
		InventorySlot->Index = i;
		InventorySlot->InventoryInterface = BackpackComp;

		Hotbar->AddChildToUniformGrid(InventorySlot, 0, i);
		InventorySlot->FlushItemData(BackpackComp->GetItemStack_Implementation(i));
	}

	Inventory->ClearChildren();
	const int32 Number = BackpackComp->GetSizeInventory_Implementation() - 9;
	for (int32 i = 0; i < Number; ++i)
	{
		USlot* InventorySlot = CreateWidget<USlot>(this, SlotClass);
		InventorySlot->Parent = this;
		InventorySlot->Index = i + 9;
		InventorySlot->InventoryInterface = BackpackComp;

		Inventory->AddChildToUniformGrid(InventorySlot, i / 9, i % 9);
		InventorySlot->FlushItemData(BackpackComp->GetItemStack_Implementation(i + 9));
	}
}

void UContainer::FlushHangItem_Implementation(const FItemStack& NewItemStack)
{
	if (IsValid(HangItem))
	{
		HangItem->FlushItemData(NewItemStack);

		if (NewItemStack.GetItemID() == AIR)
		{
			HangItem->RemoveFromParent();
			HangItem = nullptr;
		}
	}
	else
	{
		HangItem = CreateWidget<UDroppableInventoryCellWidget>(this, HangItemClass);
		HangItem->FlushItemData(NewItemStack);
		UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel->AddChildToCanvas(HangItem);
		CanvasPanelSlot->SetSize(HangImageSize);
		HangItem->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UContainer::OnBagItemUpdateDelegateEvent(int32 Index, const FItemStack& NewItemStack)
{
	if (USlot* InventorySlot = Cast<USlot>(Inventory->GetChildAt(Index - 9)))
	{
		InventorySlot->FlushItemData(NewItemStack);
	}
}

void UContainer::OnHotbarItemUpdateDelegateEvent(int32 Index, const FItemStack& NewItemStack)
{
	if (USlot* InventorySlot = Cast<USlot>(Hotbar->GetChildAt(Index)))
	{
		InventorySlot->FlushItemData(NewItemStack);
	}
}

void UContainer::OnHangItemUpdateEvent(const FItemStack& NewItemStack)
{
	FlushHangItem(NewItemStack);
}

void UContainer::NativeDestruct()
{
	Super::NativeDestruct();

	SetInputModeGameOnly();
}

FReply UContainer::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::E || Key == EKeys::Escape)
	{
		RemoveFromParent();
	}

	return FReply::Handled();
}

void UContainer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(HangItem))
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(HangItem->Slot))
		{
			FVector2D Location;
			UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), Location.X, Location.Y);

			CanvasPanelSlot->SetPosition(Location - (HangImageSize / 2.f));
		}
	}
}

void UContainer::SlotClick_Implementation(USlot* ClickedSlot, EMouseEvent MouseEvent)
{
	const int32 ClickedIndex = ClickedSlot->Index;
	TScriptInterface<UInventoryInterface> InventoryInterface = ClickedSlot->InventoryInterface;

	FItemStack ClickedItemStack = IInventoryInterface::Execute_GetItemStack(InventoryInterface.GetObject(), ClickedIndex);
	FItemStack HangItemStack = BackpackComp->GetHangItemStack();

	if (ClickedItemStack.IsEmpty())
	{
		if (!HangItemStack.IsEmpty() && ClickedSlot->IsItemValid(HangItemStack))
		{
			const int32 Count = MouseEvent == EMouseEvent::LMB ? HangItemStack.GetCount() : 1;
			IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, HangItemStack.SplitStack(Count));
			BackpackComp->SetHangItemStack(HangItemStack);
		}
	}
	else
	{
		if (HangItemStack.IsEmpty())
		{
			const int32 Count = MouseEvent == EMouseEvent::LMB ? ClickedItemStack.GetCount() : (ClickedItemStack.GetCount() + 1) / 2;
			FItemStack DecrItemStack = IInventoryInterface::Execute_DecrStackSize(InventoryInterface.GetObject(), ClickedIndex, Count);
			BackpackComp->SetHangItemStack(DecrItemStack);

			ClickedSlot->OnTake();
		}
		else
		{
			if (ClickedItemStack.GetItemID() == HangItemStack.GetItemID())
			{
				int32 Count = MouseEvent == EMouseEvent::LMB ? HangItemStack.GetCount() : 1;

				if (Count > HangItemStack.GetMaxStackSize() - ClickedItemStack.GetCount())
				{
					Count = HangItemStack.GetMaxStackSize() - ClickedItemStack.GetCount();
				}

				HangItemStack.Shrink(Count);
				ClickedItemStack.Grow(Count);
				IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, ClickedItemStack);
				BackpackComp->SetHangItemStack(HangItemStack);
			}
			else
			{
				IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, HangItemStack);
				BackpackComp->SetHangItemStack(ClickedItemStack);
			}
		}
	}
}
