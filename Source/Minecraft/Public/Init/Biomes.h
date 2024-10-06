// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Biomes.generated.h"

class UBiome;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBiomes : public UObject
{
	GENERATED_BODY()
public:
	~UBiomes();

	FORCEINLINE static UBiomes& Get()
	{
		if (Instance == nullptr)
		{
			Initialization();
		}
		return *Instance;
	}

	static void Initialization();

private:
	static UBiome* GetRegisteredBiome(const FName& Name);

private:
	static UBiomes* Instance;
};
