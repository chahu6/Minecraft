// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Crafting/CraftingResultComponent.h"
#include "Utils/ItemStackHelper.h"

UCraftingResultComponent::UCraftingResultComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCraftingResultComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}

void UCraftingResultComponent::Init()
{
	StackResult.Init(FItemStack(), 1);
}

void UCraftingResultComponent::SetRecipeUsed(const TSharedPtr<IRecipe>& InRecipe)
{
	RecipeUsed = InRecipe;
}

TSharedPtr<IRecipe> UCraftingResultComponent::GetRecipeUsed() const
{
	return RecipeUsed;
}

FItemStack UCraftingResultComponent::GetItemStack_Implementation(int32 Index) const
{
	return StackResult[0];
}

FItemStack UCraftingResultComponent::RemoveStackFromSlot_Implementation(int32 Index)
{
	return ItemStackHelper::GetAndRemove(StackResult, 0);
}

void UCraftingResultComponent::SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack)
{
	StackResult[0] = Stack;
}

