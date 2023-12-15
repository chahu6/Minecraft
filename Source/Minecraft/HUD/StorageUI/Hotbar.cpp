#include "Hotbar.h"
#include "Minecraft/Entity/Player/MCPlayer.h"
#include "Minecraft/MinecraftComponents/Storage/BackpackComponent.h"

void UHotbar::NativePreConstruct()
{
	Super::NativePreConstruct();

	AMCPlayer* Player = Cast<AMCPlayer>(GetOwningPlayerPawn());
	if (Player)
	{
		Backpack = Player->GetComponentByClass<UBackpackComponent>();
	}

	if (Backpack)
	{
		Backpack->OnHotbarUpdate.AddUObject(this, &UHotbar::FlushHotbar);
	}

	if (Player)
	{
		Player->OnSwitchMainHand.AddUObject(this, &UHotbar::FlushSelectedBox);
	}
}

void UHotbar::NativeConstruct()
{
	Super::NativeConstruct();

	FlushHotbar();
}

void UHotbar::NativeDestruct()
{
	Super::NativeDestruct();
}