#include "UI/Widget/StorageUI/Hotbar.h"
#include "Entity/MinecraftPlayer.h"
//#include "Components/Storage/BackpackComponent.h"

void UHotbar::NativePreConstruct()
{
	Super::NativePreConstruct();

	AMinecraftPlayer* Player = Cast<AMinecraftPlayer>(GetOwningPlayerPawn());
	if (Player)
	{
		//Backpack = Player->GetComponentByClass<UBackpackComponent>();
	}

	//if (Backpack)
	{
		//Backpack->OnHotbarUpdate.AddUObject(this, &UHotbar::FlushHotbar);
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
