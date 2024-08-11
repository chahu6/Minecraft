// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TerrainComponent.generated.h"

class AChunk;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UTerrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTerrainComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void LoadTerrainInfo(AChunk* Chunk);

	void LoadTerrainBlockID(AChunk* Chunk);
};
