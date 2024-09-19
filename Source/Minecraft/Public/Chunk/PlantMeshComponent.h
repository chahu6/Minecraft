// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "PlantMeshComponent.generated.h"

class AChunk;
struct FMeshData;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UPlantMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:
	UPlantMeshComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void Render(const TMap<int32, TSharedPtr<FMeshData>>& NewMeshDatas);

	//void BuildMesh();

private:
	UPROPERTY()
	TObjectPtr<AChunk> Chunk;
};
