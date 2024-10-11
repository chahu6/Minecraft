// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileEntity.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API ATileEntity : public AActor
{
	GENERATED_BODY()
public:
	ATileEntity();

protected:
	virtual void BeginPlay() override;
};
