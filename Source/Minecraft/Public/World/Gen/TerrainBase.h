// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TerrainBase.generated.h"

struct FChunkPos;
class AWorldManager;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MINECRAFT_API UTerrainBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() {};

	UFUNCTION(BlueprintNativeEvent, Category = "Terrain Generator")
	void Generate(AWorldManager* InWorldManager, const FChunkPos& InChunkPos);
	virtual void Generate_Implementation(AWorldManager* InWorldManager, const FChunkPos& InChunkPos) {};

	void SetSeed(int32 NewSeed);

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 Seed = 0;

public:
	FORCEINLINE int32 GetSeed() const { return Seed; }
};