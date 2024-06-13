// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "BlockItem.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBlockItem : public UItem
{
	GENERATED_BODY()
public:
	virtual UItem* Clone() override;
};
