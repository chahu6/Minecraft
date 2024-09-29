// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MCGameMode.generated.h"

class AEntityPlayer;
class AWorldManager;
class UProgressBarWidget;

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMCGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreInitializeComponents() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void EnterWorld(APlayerController* NewPlayer);

	void SetPrograssPercent(float Percent);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWorldManager> WorldManagerClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UProgressBarWidget> ProgressBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UProgressBarWidget> ProgressBarWidget;
};
