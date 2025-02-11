// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshBuilder.h"

class UBrickRenderComponent;

struct FBrickVertex
{
	uint8 X;
	uint8 Y;
	uint8 Z;
	uint8 AmbientOcclusionFactor;

	FBrickVertex()
		:X(0), Y(0), Z(0), AmbientOcclusionFactor(0)
	{}

	FBrickVertex(const FIntVector& InCoordinates, uint8 InAmbientOcclusionFactor)
		: X(InCoordinates.X), Y(InCoordinates.Y), Z(InCoordinates.Z), AmbientOcclusionFactor(InAmbientOcclusionFactor)
	{}
};

/** Vertex Buffer */
class FBrickChunkVertexBuffer : public FVertexBuffer
{
public:
	TArray<FBrickVertex> Vertices;

	virtual void InitRHI() override
	{
		if (!Vertices.IsEmpty())
		{
			FRHIResourceCreateInfo CreateInfo(TEXT("FBrickChunkVertexBuffer"));
			VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FBrickVertex), BUF_Dynamic, CreateInfo);

			void* VertexBufferData = RHILockBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FBrickVertex), EResourceLockMode::RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FBrickVertex));
			RHIUnlockBuffer(VertexBufferRHI);
		}
	}
};

/** Index Buffer */
class FBrickChunkIndexBuffer : public FIndexBuffer
{
public:
	TArray<uint16> Indices;

	virtual void InitRHI() override
	{
		if (!Indices.IsEmpty())
		{
			FRHIResourceCreateInfo CreateInfo(TEXT("FBrickChunkIndexBuffer"));
			IndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), Indices.Num() * sizeof(uint16), BUF_Static, CreateInfo);

			void* VertexBufferData = RHILockBuffer(IndexBufferRHI, 0, Indices.Num() * sizeof(uint16), EResourceLockMode::RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, Indices.GetData(), Indices.Num() * sizeof(uint16));
			RHIUnlockBuffer(IndexBufferRHI);
		}
	}
};

class FBrickChunkVertexFactory : public FLocalVertexFactory
{
public:
	FBrickChunkVertexFactory(ERHIFeatureLevel::Type InFeatureLevel, const char* InDebugName)
		:FLocalVertexFactory(InFeatureLevel, InDebugName)
	{}

	void Init(FBrickChunkVertexBuffer* InVertexBuffer)
	{
		if (IsInRenderingThread())
		{
			FDataType NewData;
			NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_UByte4N);
			NewData.TextureCoordinates.Add(STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_UByte4N));
			NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_Color);
			SetData(NewData);
		}
		else
		{
			ENQUEUE_RENDER_COMMAND(InitBrickChunkVertexFactory)([InVertexBuffer, this](FRHICommandListImmediate& THICmdList) {
				FDataType NewData;
				NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_UByte4N);
				NewData.TextureCoordinates.Add(STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_UByte4N));
				NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_Color);
				SetData(NewData);
			});
		}
	}
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

private:
	//FBrickChunkVertexBuffer VertexBuffer;
	FStaticMeshVertexBuffers VertexBuffers;
	FBrickChunkIndexBuffer IndexBuffer;
	FBrickChunkVertexFactory VertexFactory;
	//FLocalVertexFactory VertexFactory;

	FVector Origin;

	FColor WireframeColor;

	FMaterialRelevance MaterialRelevance;
};