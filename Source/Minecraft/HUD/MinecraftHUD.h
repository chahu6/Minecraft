// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MinecraftHUD.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMinecraftHUD : public AHUD
{
	GENERATED_BODY()

private:
	friend class AMCPlayerController;

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

public:
	void AddDebugInfo();
	void RemoveDebugInfo();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DebugInfoClass;

	UPROPERTY()
	class UUserWidget* DebugInfoUI;

};
