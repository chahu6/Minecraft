// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "ItemAir.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UItemAir : public UItem
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<UBlock> Air;

};
