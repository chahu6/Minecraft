// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MinecraftSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void LoadPrimaryAssets() const;
};
