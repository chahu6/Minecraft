// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MCGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWorldManager> WorldManagerClass;
};
