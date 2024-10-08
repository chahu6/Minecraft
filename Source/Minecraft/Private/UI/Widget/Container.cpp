// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Container.h"
#include "Player/EntityPlayer.h"
#include "Components/Inventory/BackpackComponent.h"
#include "Interfaces/InventoryInterface.h"

void UContainer::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsFocusable(true);

	Player = GetOwningPlayerPawn<AEntityPlayer>();
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

void UContainer::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetIgnoreLookInput(false);

		Player->CloseContainer();
	}
}

void UContainer::OnContainerClosed(AEntityPlayer* PlayerIn)
{
	UBackpackComponent* Inventory = Player->GetComponentByClass<UBackpackComponent>();
	if (Inventory)
	{
		FItemStack ItemStack = Inventory->GetItemStack();
		if (ItemStack.IsEmpty()) return;

	}
}

void UContainer::ClearContainer(AEntityPlayer* PlayerIn, IInventoryInterface* InventoryIn)
{
	//PlayerIn->DropItem();

	//for (int32 i = 0; i < InventoryIn->GetSizeInventory(); ++i)
	//{
	//	PlayerIn->GetComponentByClass<UBackpackComponent>()->PlaceItemBackInInventory(InventoryIn->RemoveStackFromSlot(i));
	//}
}

void UContainer::HandleLMB_Implementation(UInventoryItem* InventoryItem)
{
}

void UContainer::HandleRMB_Implementation(UInventoryItem* InventoryItem)
{
}
