// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BagWidgetController.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
//#include "Components/Inventory/BackpackComponent.h"

void UBagWidgetController::BroadcastInitialValue()
{
}

void UBagWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	CraftingComp = Pawn->GetComponentByClass<UCraftingComponent>();
	CraftingResultComp = Pawn->GetComponentByClass<UCraftingResultComponent>();

	//BackpackComp->OnInventoryItemUpdateDelegate.AddLambda([this](int32 Index, const FItemStack& NewItemStack) {

	//});

	CraftingComp->OnCraftingResultDelegate.AddLambda([this](const FItemStack& CraftingResult) {
		CraftingResultComp->SetInventorySlotContents_Implementation(0, CraftingResult);
		OnCraftingResultUpdateDelegate.Broadcast(0, CraftingResult);
	});

	CraftingComp->OnCraftingMatrixDelegate.AddLambda([this]() {
		OnCraftingMatrixUpdateDelegate.Broadcast();
	});
}

void UBagWidgetController::BeginDestroy()
{
	Super::BeginDestroy();

	//CraftingComp->OnCraftingResultDelegate.
}
