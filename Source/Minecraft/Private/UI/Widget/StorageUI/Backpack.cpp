#include "UI/Widget/StorageUI/Backpack.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
#include "Player/EntityPlayer.h"

#include "UI/Widget/StorageUI/InventoryItem.h"

void UBackpack::NativePreConstruct()
{
	Player = Player == nullptr ? GetOwningPlayerPawn<AEntityPlayer>() : Player;

	Super::NativePreConstruct();
	
	if (Player)
	{
		CraftingSystem = Player->GetComponentByClass<UCraftingComponent>();
		CraftingResult = Player->GetComponentByClass<UCraftingResultComponent>();
	}

	if (CraftingSystem)
	{
		CraftingSystem->OnCraftingItem.AddUObject(this, &UBackpack::OnCraftMatrixChanged);
	}
}

void UBackpack::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBackpack::InitUI()
{
	Super::InitUI();

	FlushCrafting();
}

void UBackpack::OnCraftMatrixChanged()
{
	SlotChangedCraftingGrid(Player, CraftingSystem, CraftingResult);
	FlushCrafting();
}

void UBackpack::OnHandleLMB(UInventoryItem* InventoryItem)
{
	if (UCraftingResultComponent* Result = Cast<UCraftingResultComponent>(InventoryItem->Inventory.GetObject()))
	{
		CraftingSystem->ShrinkAllItems();
	}
}

void UBackpack::OnContainerClosed(AEntityPlayer* PlayerIn)
{
	Super::OnContainerClosed(PlayerIn);

	ClearContainer(PlayerIn, CraftingSystem);
}