#include "InventoryComponent.h"
#include "Minecraft/HUD/Inventory.h"
#include "Minecraft/Subsystem/MCGameInstanceSubsystem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Items.SetNum(InventorySize);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UInventoryComponent::AddItemToInventory(int32 ID, int32 Num)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("添加物品"));

	int32 Index = -1;
	if (FindItemIndex(ID, Index))
	{
		Items[Index].Count += Num;
		return true;
	}
	
	if (AnyEmptySlots(Index))
	{
		Items[Index] = { ID, Num };
		return true;
	}

	return false;
}

void UInventoryComponent::TransferSlots(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex)
{
	FItemSlot SlotItem = SourceInventory->GetItems()[SourceIndex];
	if (Items.IsValidIndex(DestinationIndex))
	{
		if (Items[DestinationIndex].ID == SlotItem.ID)
		{

		}
		else
		{
			SourceInventory->GetItems()[SourceIndex] = Items[DestinationIndex];
			Items[DestinationIndex] = SlotItem;
		}

		OnInventoryUpdate.Broadcast();
	}
}

bool UInventoryComponent::FindItemIndex(int32 ID, int32& Index)
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UMCGameInstanceSubsystem* MCGameInstance = GameInstance->GetSubsystem<UMCGameInstanceSubsystem>();
	FItemDetails* ItemDetails = MCGameInstance->GetItemDataTable()->FindRow<FItemDetails>(FName(*FString::FromInt(ID)), TEXT("UInventoryComponent"));

	Index = -1;
	if (!ItemDetails->bIsStack) return false;

	for (auto Itr = Items.CreateConstIterator(); Itr; ++Itr)
	{
		if (Itr->ID == ID)
		{
			if (Itr->Count < ItemDetails->MaxCount)
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
	for (auto Itr = Items.CreateConstIterator(); Itr; ++Itr)
	{
		if (Itr->ID == -1)
		{
			Index = Itr.GetIndex();
			return true;
		}
	}

	Index = -1;
	return false;
}
