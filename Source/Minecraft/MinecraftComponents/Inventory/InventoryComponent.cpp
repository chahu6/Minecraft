#include "InventoryComponent.h"
#include "Minecraft/HUD/Inventory.h"
#include "Minecraft/Subsystem/MCGameInstanceSubsystem.h"
#include "Minecraft/Item/Item.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemsData.SetNum(InventorySize);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UInventoryComponent::AddItemToInventory(const AItem* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("添加物品"));

	int32 Index = -1;
	if (Item->GetItemData().MaxCount != 0)
	{
		if (FindItemIndex(Item, Index))
		{
			ItemsData[Index].Count += Item->GetItemData().Count;
			return true;
		}
	}
	
	if (AnyEmptySlots(Index))
	{
		ItemsData[Index] = Item->GetItemData();
		return true;
	}

	return false;
}

void UInventoryComponent::TransferSlots(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex)
{
	FItemSlot SlotItem = SourceInventory->GetItems()[SourceIndex];
	if (ItemsData.IsValidIndex(DestinationIndex))
	{
		if (ItemsData[DestinationIndex].ID == SlotItem.ID)
		{

		}
		else
		{
			SourceInventory->GetItems()[SourceIndex] = ItemsData[DestinationIndex];
			ItemsData[DestinationIndex] = SlotItem;
		}

		OnInventoryUpdate.Broadcast();
	}
}

void UInventoryComponent::CreateItemToWorld(FName ID, int32 Quantity)
{
	if (ItemDataTable == nullptr) return;
	FItemDetails* ItemDetails = ItemDataTable->FindRow<FItemDetails>(ID, TEXT("UInventoryComponent::CreateItemToWorld"));
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 300;
		AItem* Item = World->SpawnActor<AItem>(AItem::StaticClass(), Location, FRotator::ZeroRotator);
		Item->SetItemData(ItemDetails, Quantity);
	}
}

void UInventoryComponent::RemoveItemFromInventory(int32 Index)
{
	if (ItemsData.IsValidIndex(Index))
	{
		ItemsData[Index].Empty();

		OnInventoryUpdate.Broadcast();
	}
}

bool UInventoryComponent::FindItemIndex(const AItem* Item, int32& Index)
{
	const FItemSlot& ItemData = Item->GetItemData();

	Index = -1;
	for (auto Itr = ItemsData.CreateConstIterator(); Itr; ++Itr)
	{
		if (Itr->ID == ItemData.ID)
		{
			if (Itr->Count < ItemData.MaxCount)
			{
				Index = Itr.GetIndex();
				return true;
			}
		}
	}

	return false;
}

bool UInventoryComponent::AnyEmptySlots(int32& Index)
{
	Index = -1;

	for (auto Itr = ItemsData.CreateConstIterator(); Itr; ++Itr)
	{
		if (Itr->ID == -1)
		{
			Index = Itr.GetIndex();
			return true;
		}
	}

	return false;
}
