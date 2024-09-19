// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MinecraftGameInstance.generated.h"

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
	void CallbackFunction();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWorldManager> WorldManagerClass;
};
