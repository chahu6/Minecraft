// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Slot.h"
#include "SlotCrafting.generated.h"

class UCraftingComponent;

/**
 * 
 */
UCLASS()
class MINECRAFT_API USlotCrafting : public USlot
{
	GENERATED_BODY()
public:
	virtual bool IsItemValid_Implementation(const FItemStack& ItemStack) const override;

	virtual void OnTake_Implementation() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TScriptInterface<UCraftingComponent> CraftMatrix;
};
