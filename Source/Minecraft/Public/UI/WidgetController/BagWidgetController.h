// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "BagWidgetController.generated.h"

class UCraftingComponent;
class UCraftingResultComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCraftingMatrixUpdateSignature);

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBagWidgetController : public UInventoryWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintAssignable)
	FOnCraftingMatrixUpdateSignature OnCraftingMatrixUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdateSignature OnCraftingResultUpdateDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UCraftingComponent> CraftingComp;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UCraftingResultComponent> CraftingResultComp;

};
