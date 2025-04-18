// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshBuilder.h"
#include "DataDrivenShaderPlatformInfo.h"
//#include "RenderResource.h"
#include "BrickGridVertexFactory.h"

class UBrickRenderComponent;
struct FVoxelProcMeshBuffers;

struct FVoxelVertex
{
	uint8 X;
	uint8 Y;
	uint8 Z;
	uint8 AmbientOcclusionFactor;

	FVoxelVertex() {}
};

/** Vertex Buffer */
class FVoxelChunkVertexBuffer : public FVertexBuffer
{
public:
	TArray<FVector3f> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(TEXT("FVoxelChunkVertexBuffer"));
		VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FVector3f), EBufferUsageFlags::Static, CreateInfo);

		void* Buffer = RHILockBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FVector3f), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Vertices.GetData(), Vertices.Num() * sizeof(FVector3f));
		RHIUnlockBuffer(VertexBufferRHI);
	}
};

class FVoxelChunkIndexBuffer : public FIndexBuffer
{
public:
	TArray<uint16> Indices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(TEXT("FVoxelChunkIndexBuffer"));
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), Indices.Num() * sizeof(uint16), BUF_Static, CreateInfo);

		void* Buffer = RHILockBuffer(IndexBufferRHI, 0, Indices.Num() * sizeof(uint16), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(uint16));
		RHIUnlockBuffer(IndexBufferRHI);
	}
};

class FVoxelChunkUVBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI() override
	{
		TResourceArray<FVector2DHalf> Data;
		Data = {
			{ 0.0f,  0.0f }, { 1.0f,  0.0f }, { 1.0f,  1.0f }, { 1.0f,  1.0f }, { 0.0f,  1.0f }, { 0.0f,  0.0f },
			{ 0.0f,  0.0f }, { 1.0f,  1.0f }, { 1.0f,  0.0f }, { 1.0f,  1.0f }, { 0.0f,  0.0f }, { 0.0f,  1.0f },
			{ 1.0f,  0.0f }, { 0.0f,  1.0f }, { 1.0f,  1.0f }, { 0.0f,  1.0f }, { 1.0f,  0.0f }, { 0.0f,  0.0f },
			{ 1.0f,  0.0f }, { 1.0f,  1.0f }, { 0.0f,  1.0f }, { 0.0f,  1.0f }, { 0.0f,  0.0f }, { 1.0f,  0.0f },
			{ 0.0f,  1.0f }, { 1.0f,  0.0f }, { 1.0f,  1.0f }, { 1.0f,  0.0f }, { 0.0f,  1.0f }, { 0.0f,  0.0f },
			{ 0.0f,  1.0f }, { 1.0f,  1.0f }, { 1.0f,  0.0f }, { 1.0f,  0.0f }, { 0.0f,  0.0f }, { 0.0f,  1.0f },
		};
		FRHIResourceCreateInfo CreateInfo(TEXT("FVoxelChunkUVBuffer"));
		VertexBufferRHI = RHICreateVertexBuffer(Data.Num() * sizeof(FVector2DHalf), EBufferUsageFlags::Static, CreateInfo);

		void* Buffer = RHILockBuffer(VertexBufferRHI, 0, Data.Num() * sizeof(FVector2DHalf), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Data.GetData(), Data.Num() * sizeof(FVector2DHalf));
		RHIUnlockBuffer(VertexBufferRHI);
	}
};

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

	/*
	* Test
	*/
	FVoxelChunkVertexBuffer VertexBuffer;
	FVoxelChunkIndexBuffer IndexBuffer;
	FBrickGridVertexFactory VertexFactory;

	TUniquePtr<FVertexBuffer> UVBuffer;
	FShaderResourceViewRHIRef UVBufferSRV;

	FMaterialRenderProxy* MyMaterialRenderProxy;
};