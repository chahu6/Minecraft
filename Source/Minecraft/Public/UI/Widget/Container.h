// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/View.h"
#include "Container.generated.h"

class UInventoryInterface;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UContainer : public UView
{
	GENERATED_BODY()
public:
	void SetInventoryInterface(TScriptInterface<UInventoryInterface> InInventoryInterface);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (ExposeOnSpawn = "true"))
	TScriptInterface<UInventoryInterface> InventoryInterface;
};
