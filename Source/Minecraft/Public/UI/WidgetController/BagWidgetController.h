// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "BagWidgetController.generated.h"

class UBackpackComponent;

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

};
