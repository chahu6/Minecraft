#include "UI/Widget/StorageUI/Backpack.h"
#include "Components/Inventory/BackpackComponent.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Entity/MinecraftPlayer.h"

UBackpack::UBackpack(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UBackpack::NativePreConstruct()
{
	Super::NativePreConstruct();

	Player = Cast<AMinecraftPlayer>(GetOwningPlayerPawn());
	if (Player)
	{
		Backpack = Player->GetBackpackComponent();
		CraftingSystem = Player->GetCraftingComponent();
	}

	if (Backpack)
	{
		Backpack->OnHotbarUpdate.AddDynamic(this, &UBackpack::FlushHotbar);
		Backpack->OnInventoryUpdate.AddDynamic(this, &UBackpack::FlushBackpack);
	}

	if (CraftingSystem)
	{
		CraftingSystem->OnCraftingItem.AddUObject(this, &UBackpack::FlushCrafting);
	}
}

void UBackpack::NativeConstruct()
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

	InitUI();
}

void UBackpack::NativeDestruct()
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

FReply UBackpack::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::E || Key == EKeys::Escape)
	{
		RemoveFromParent();
	}

	return FReply::Handled();
}

void UBackpack::InitUI()
{
	FlushHotbar();
	FlushBackpack();
	FlushCrafting();
}
