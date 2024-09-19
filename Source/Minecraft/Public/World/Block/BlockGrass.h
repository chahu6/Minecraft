// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Block/Block.h"
#include "Interfaces/Block/GrowableInterface.h"
#include "BlockGrass.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBlockGrass : public UBlock, public IGrowableInterface
{
	GENERATED_BODY()
public:
	/** Growable Interface */
	virtual bool CanGrow() override;
	virtual bool CanUseBonemeal() override;
	virtual void Grow() override;
	/** end Growable Interface */

	virtual void UpdateTick() override;
};
