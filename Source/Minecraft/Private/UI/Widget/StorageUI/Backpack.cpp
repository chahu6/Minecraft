#include "UI/Widget/StorageUI/Backpack.h"
#include "Components/Inventory/BackpackComponent.h"
#include "Components/Crafting/CraftingComponent.h"

UBackpack::UBackpack()
{
	//SetIsFocusable(true);
}

void UBackpack::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	Player = GetOwningPlayerPawn();
	if (Player)
	{
		CraftingSystem = Player->GetComponentByClass<UCraftingComponent>();
		BackpackComponent = Player->GetComponentByClass<UBackpackComponent>();
	}

	if (BackpackComponent)
	{
		BackpackComponent->OnHotbarUpdate.AddDynamic(this, &UBackpack::FlushHotbar);
		BackpackComponent->OnInventoryUpdate.AddDynamic(this, &UBackpack::FlushBackpack);
	}

	if (CraftingSystem)
	{
		CraftingSystem->OnCraftingItem.AddUObject(this, &UBackpack::FlushCrafting);
	}
}

void UBackpack::NativeConstruct()
{
	Super::NativeConstruct();

	InitUI();
}

void UBackpack::NativeDestruct()
{
	Super::NativeDestruct();

}

FReply UBackpack::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	/*FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::E || Key == EKeys::Escape)
	{
		RemoveFromParent();
	}*/

	return FReply::Handled();
}

void UBackpack::InitUI()
{
	FlushHotbar();
	FlushBackpack();
	FlushCrafting();
}
