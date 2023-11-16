// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

class AMCPlayer;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

private:
	void ShowDebugInfo();

	void RayCast();
	
private:
	UPROPERTY()
	class AMinecraftHUD* MinecraftHUD;

	AMCPlayer* MCPlayer = nullptr;

	bool bIsOpened = false;
};
