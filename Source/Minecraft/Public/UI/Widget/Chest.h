// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Inventory.h"
#include "Chest.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UChest : public UInventory
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Chest;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (ExposeOnSpawn = true))
	TObjectPtr<UInventoryComponent> ChestInventory;
};
