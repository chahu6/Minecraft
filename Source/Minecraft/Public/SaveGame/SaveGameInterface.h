// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFT_API ISaveGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void LoadGameData(bool bAsync);

	UFUNCTION(BlueprintNativeEvent)
	void SaveGameData(bool bAsync);

	UFUNCTION(BlueprintNativeEvent)
	void GetGameData();
};
