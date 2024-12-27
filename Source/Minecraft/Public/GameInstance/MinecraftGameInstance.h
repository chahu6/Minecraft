// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveGame/SaveGameInterface.h"
#include "MinecraftGameInstance.generated.h"

struct FStreamableHandle;
class UWorldSaveGame;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftGameInstance : public UGameInstance, public ISaveGameInterface
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	/** SaveGame Interface */
	virtual void LoadGameData_Implementation(bool bAsync) override;
	virtual void SaveGameData_Implementation(bool bAsync) override;
	/** SaveGame Interface end*/

private:
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWorldSaveGame> WorldData;
};
