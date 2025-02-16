// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshBuilder.h"
#include "DataDrivenShaderPlatformInfo.h"

class UBrickRenderComponent;
struct FVoxelProcMeshBuffers;

class FVoxelProcMeshBuffersRenderData
{
public:
	FLocalVertexFactory VertexFactory;
	const TSharedPtr<const FVoxelProcMeshBuffers> Buffers;

	~FVoxelProcMeshBuffersRenderData();

	static TSharedRef<FVoxelProcMeshBuffersRenderData> GetRenderData(const TSharedRef<const FVoxelProcMeshBuffers>& InBuffers, ERHIFeatureLevel::Type InFeatureLevel);
private:
	explicit FVoxelProcMeshBuffersRenderData(const TSharedRef<const FVoxelProcMeshBuffers>& InBuffers, ERHIFeatureLevel::Type InFeatureLevel);
};

struct FVoxelProcMeshProxySection
{
	TSharedPtr<FVoxelProcMeshBuffers> Buffers;
	TSharedPtr<FVoxelProcMeshBuffersRenderData> RenderData;
};

/**
 * 
 */
class FBrickChunkSceneProxy : public FPrimitiveSceneProxy
{
public:
	FBrickChunkSceneProxy(UBrickRenderComponent* InComponent);
	virtual ~FBrickChunkSceneProxy();

	SIZE_T GetTypeHash() const override;

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override;

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	virtual void OnTransformChanged() override;

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }
	SIZE_T GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

	void BuildFace(const FVector& InBlockPos);

	virtual void CreateRenderThreadResources() override;

	virtual void DestroyRenderThreadResources() override;

private:
	inline void InitOrUpdateResource(FRenderResource* Resource)
	{
		if (!Resource->IsInitialized())
		{
			Resource->InitResource();
		}
		else
		{
			Resource->UpdateRHI();
		}
	}

	FMeshBatch& DrawSection(FMeshElementCollector& Collector, const FVoxelProcMeshProxySection& Section, const FMaterialRenderProxy* MaterialRenderProxy, bool bEnableTessellation, bool bWireframe) const;

private:
	UBrickRenderComponent* const Component;
	const FMaterialRelevance MaterialRelevance;

	TArray<FVoxelProcMeshProxySection> Sections;

	FVector Origin;

	FColor WireframeColor;
};