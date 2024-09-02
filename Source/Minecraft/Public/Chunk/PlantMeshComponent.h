// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "PlantMeshComponent.generated.h"

class AChunk;

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
	void Render();

	void BuildMesh();

private:
	UPROPERTY()
	TObjectPtr<AChunk> Chunk;

	TSharedPtr<struct FMeshData> MeshData;
};
