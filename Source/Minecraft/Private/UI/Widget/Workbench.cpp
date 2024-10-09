// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Workbench.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
#include "Player/EntityPlayer.h"

UWorkbench::UWorkbench()
{
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("Crafting"));
	CraftingComponent->SetInventoryWidth(3);
	CraftingComponent->SetInventoryHeight(3);
	CraftingComponent->Init();

	CraftingResultComponent = CreateDefaultSubobject<UCraftingResultComponent>(TEXT("CraftingResult"));
	//CraftingResultComponent->Init();
}

void UWorkbench::NativePreConstruct()
{
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