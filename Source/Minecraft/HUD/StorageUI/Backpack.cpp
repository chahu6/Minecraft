#include "Backpack.h"
#include "Minecraft/MinecraftComponents/Storage/BackpackComponent.h"
#include "Minecraft/Entity/Player/MCPlayer.h"

UBackpack::UBackpack(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;
}

void UBackpack::NativePreConstruct()
{
	Super::NativePreConstruct();

	Player = GetOwningPlayerPawn();
	if (Player)
	{
		Backpack = Player->GetComponentByClass<UBackpackComponent>();
	}

	if (Backpack)
	{
		Backpack->OnHotbarUpdate.AddUObject(this, &UBackpack::FlushHotbar);
		Backpack->OnInventoryUpdate.AddUObject(this, &UBackpack::FlushBackpack);
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
}

void UBackpack::HangItemStackToMouse(int32 Index)
{
	AMCPlayer* MCCharacter = Cast<AMCPlayer>(Player);
	if (MCCharacter)
	{
		const UBackpackComponent* BackpackComponent = MCCharacter->GetBackpackComponent();
		HangItemStack = BackpackComponent->GetItemStack(Index);
	}
}
