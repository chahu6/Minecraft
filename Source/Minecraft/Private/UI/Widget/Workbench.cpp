// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Workbench.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
#include "Player/EntityPlayer.h"
#include "UI/Widget/StorageUI/InventoryItem.h"

UWorkbench::UWorkbench()
{
}

void UWorkbench::NativePreConstruct()
{
	CraftingComponent = NewObject<UCraftingComponent>();
	CraftingComponent->SetInventoryWidth(3);
	CraftingComponent->SetInventoryHeight(3);
	CraftingComponent->Init();

	CraftingResultComponent = NewObject<UCraftingResultComponent>();
	CraftingResultComponent->Init();

	Player = Player == nullptr ? GetOwningPlayerPawn<AEntityPlayer>() : Player;

	Super::NativePreConstruct();

	if (CraftingComponent)
	{
		CraftingComponent->OnCraftingItem.AddUObject(this, &UWorkbench::OnCraftMatrixChanged);
	}

	/*if (CraftingResultComponent)
	{
		CraftingResultComponent->OnCraftingResult.AddUObject(this, &UWorkbench::FlushCraftingResult);
	}*/
}

void UWorkbench::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWorkbench::OnCraftMatrixChanged()
{
	SlotChangedCraftingGrid(Player, CraftingComponent, CraftingResultComponent);
	FlushCrafting();
}

void UWorkbench::OnHandleLMB(UInventoryItem* InventoryItem)
{
	if (UCraftingResultComponent* Result = Cast<UCraftingResultComponent>(InventoryItem->Inventory.GetObject()))
	{
		CraftingComponent->ShrinkAllItems();
	}
}
