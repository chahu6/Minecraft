// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "BrickRenderComponent.generated.h"

struct FVoxelProcMeshBuffers;

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class BRICKGRID_API UBrickRenderComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

	friend class FBrickChunkSceneProxy;

public:
	UBrickRenderComponent();
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	int32 AddProcMeshSection(TUniquePtr<FVoxelProcMeshBuffers> Buffers);
	void SetProcMeshSection(int32 Index, TUniquePtr<FVoxelProcMeshBuffers> Buffers);

	UFUNCTION(BlueprintCallable)
	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals);

	UPROPERTY(EditAnywhere, Category = "Debug")
	FColor WireframeColor;

	UMaterialInterface* GetMyMaterial() const;
	virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const override;

private:
	void UpdateLocalBounds();

private:
	struct FVoxelProcMeshSection
	{
		TSharedPtr<FVoxelProcMeshBuffers> Buffers;
	};

	TArray<FVoxelProcMeshSection> ProcMeshSections;
	FBoxSphereBounds LocalBounds;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	TObjectPtr<UMaterialInterface> MyMaterial;
};
