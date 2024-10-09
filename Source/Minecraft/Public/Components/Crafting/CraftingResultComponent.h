// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStack.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "CraftingResultComponent.generated.h"

class IRecipe;

DECLARE_MULTICAST_DELEGATE(FOnCraftingResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UCraftingResultComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UCraftingResultComponent();
	virtual void BeginPlay() override;

	/** Inventory Interface */
	virtual FItemStack GetItemStack_Implementation(int32 Index) const override;
	virtual FItemStack RemoveStackFromSlot_Implementation(int32 Index) override;
	virtual void SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack) override;
	/** end Inventory Interface */

public:
	void SetRecipeUsed(const TSharedPtr<IRecipe>& InRecipe);
	TSharedPtr<IRecipe> GetRecipeUsed() const;

	FOnCraftingResult OnCraftingResult;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStack> StackResult;

	TSharedPtr<IRecipe> RecipeUsed;
};
