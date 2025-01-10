// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BagWidgetController.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
#include "Components/Inventory/BackpackComponent.h"

void UBagWidgetController::BroadcastInitialValue()
{
}

void UBagWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	CraftingComp = Pawn->GetComponentByClass<UCraftingComponent>();
	CraftingResultComp = Pawn->GetComponentByClass<UCraftingResultComponent>();

	BackpackComp->OnInventoryItemUpdateDelegate.AddLambda([this](int32 Index, const FItemStack& NewItemStack) {
		OnBagItemUpdateDelegate.Broadcast(Index - 9, NewItemStack);
	});

	BackpackComp->OnHotbarItemUpdateDelegate.AddLambda([this](int32 Index, const FItemStack& NewItemStack) {
		OnHotbarItemUpdateDelegate.Broadcast(Index, NewItemStack);
	});

	BackpackComp->OnHangItemUpdateDelegate.AddLambda([this](const FItemStack& NewItemStack) {
		OnHangItemUpdateSignature.Broadcast(NewItemStack);
	});

	CraftingComp->OnCraftingResultDelegate.AddLambda([this](const FItemStack& CraftingResult) {
		CraftingResultComp->SetInventorySlotContents_Implementation(0, CraftingResult);
	});

	CraftingComp->OnItemUpdateDelegate.AddLambda([this](int32 Index, const FItemStack& NewItemStack) {
		OnCraftingItemUpdateDelegate.Broadcast(Index, NewItemStack);
	});

	CraftingResultComp->OnCraftingResultDelegate.AddLambda([this](int32 Index, const FItemStack& NewItemStack) {
		OnCraftingResultUpdateDelegate.Broadcast(Index, NewItemStack);
	});
}

void UBagWidgetController::BeginDestroy()
{
	Super::BeginDestroy();

	//CraftingComp->OnCraftingResultDelegate.
}
