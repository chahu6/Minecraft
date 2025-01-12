// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStack.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "CraftingResultComponent.generated.h"

class IRecipe;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UCraftingResultComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UCraftingResultComponent();
	virtual void BeginPlay() override;

	/** Inventory Interface */
	int32 GetSizeInventory_Implementation() const override;
	virtual FItemStack GetItemStack_Implementation(int32 Index) const override;
	virtual FItemStack RemoveStackFromSlot_Implementation(int32 Index) override;
	virtual void SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack) override;
	virtual FItemStack DecrStackSize_Implementation(int32 Index, int32 Count) override;
	/** end Inventory Interface */

public:
	void SetRecipeUsed(const TSharedPtr<IRecipe>& InRecipe);
	TSharedPtr<IRecipe> GetRecipeUsed() const;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdateSignature OnCraftingResultDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemStack> StackResult;

	UPROPERTY(EditDefaultsOnly)
	int32 Number = 1;

	TSharedPtr<IRecipe> RecipeUsed;
};
