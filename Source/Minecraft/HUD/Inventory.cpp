#include "Inventory.h"
#include "Minecraft/Character/MCCharacter.h"
#include "Minecraft/MinecraftComponents/Inventory/InventoryComponent.h"

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();

	Player = Cast<AMCCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		InventoryComp = Player->GetComponentByClass<UInventoryComponent>();
	}

	if (InventoryComp)
	{
		InventoryComp->OnInventoryUpdate.AddDynamic(this, &UInventory::FlushInventory);
	}
}

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}

	FlushInventory();
}

void UInventory::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}

FReply UInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::E || Key == EKeys::Escape)
	{
		RemoveFromParent();
	}

	return FReply::Handled();
}
