// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MinecraftGameInstance.generated.h"

struct FStreamableHandle;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

private:
	void LoadPrimaryAssets();

protected:
	TSharedPtr<FStreamableHandle> LoadAssetHandle;
};
