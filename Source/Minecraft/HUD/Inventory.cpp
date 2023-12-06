#include "Inventory.h"
#include "Minecraft/MinecraftComponents/Inventory/InventoryComponent.h"

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();

	Player = GetOwningPlayerPawn();
	if (Player)
	{
		InventoryComp = Player->GetComponentByClass<UInventoryComponent>();
	}

	if (InventoryComp)
	{
		InventoryComp->OnInventoryUpdate.AddUObject(this, &UInventory::FlushInventory);
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