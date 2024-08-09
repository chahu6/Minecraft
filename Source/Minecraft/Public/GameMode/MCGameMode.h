// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MCGameMode.generated.h"

class AMCPlayerController;
class AMinecraftPlayer;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreInitializeComponents() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

	virtual void BeginPlay() override;

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

	void EnterWorld(AMCPlayerController* NewPlayer);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMinecraftPlayer> MinecraftPlayerClass;
};
