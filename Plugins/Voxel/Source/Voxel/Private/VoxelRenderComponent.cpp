// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelRenderComponent.h"
#include "Materials/MaterialRenderProxy.h"
#include "MaterialDomain.h"
#include "Async/TaskGraphInterfaces.h"

const FVector FaceNormals[6] =
{
	FVector(-1, 0, 0),
	FVector(+1, 0, 0),
	FVector(0, -1, 0),
	FVector(0, +1, 0),
	FVector(0, 0, -1),
	FVector(0, 0, +1)
};

struct FVoxelVertex
{
	float X;
	float Y;
	float Z;
	float AmbientOcclusionFactor;

	FVoxelVertex() {}
};

/** Vertex Buffer */
class FVoxelChunkVertexBuffer : public FVertexBuffer
{
public:
	TArray<FVector4f> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(TEXT("FVoxelChunkVertexBuffer"));
		VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FVector4f), EBufferUsageFlags::Static, CreateInfo);

		void* Buffer = RHILockBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FVector4f), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Vertices.GetData(), Vertices.Num() * sizeof(FVector4f));
		RHIUnlockBuffer(VertexBufferRHI);
	}
};

/** Index Buffer */
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

/** Tangent Buffer */
class FVoxelChunkTangentBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(TEXT("FVoxelChunkTangentBuffer"));
		VertexBufferRHI = RHICreateVertexBuffer(12 * sizeof(FPackedNormal), BUF_Dynamic, CreateInfo);
		// Copy the vertex data into the vertex buffer.
		FPackedNormal* TangentBufferData = (FPackedNormal*)RHILockBuffer(VertexBufferRHI, 0, 12 * sizeof(FPackedNormal), RLM_WriteOnly);
		for (int32 FaceIndex = 0; FaceIndex < 6; ++FaceIndex)
		{
			const FVector UnprojectedTangentX = FVector(+1, -1, 0).GetSafeNormal();
			const FVector UnprojectedTangentY(-1, -1, -1);
			const FVector FaceNormal = FaceNormals[FaceIndex];
			const FVector ProjectedFaceTangentX = (UnprojectedTangentX - FaceNormal * (UnprojectedTangentX | FaceNormal)).GetSafeNormal();
			*TangentBufferData++ = ProjectedFaceTangentX;
			*TangentBufferData++ = FVector4(FaceNormal, FMath::Sign(UnprojectedTangentY | (FaceNormal ^ ProjectedFaceTangentX)));
		}
		RHIUnlockBuffer(VertexBufferRHI);
	}
};

TGlobalResource<FVoxelChunkTangentBuffer> TangentBuffer;

class FVoxelChunkVertexFactory : public FLocalVertexFactory
{
public:
	FVoxelChunkVertexFactory(ERHIFeatureLevel::Type InFeatureLevel)
		: FLocalVertexFactory(InFeatureLevel, "FVoxelChunkVertexFactory")
	{ }

	void Init(const FVoxelChunkVertexBuffer& InVertexBuffer, const FPrimitiveSceneProxy* InPrimitiveSceneProxy)
	{
		check(!IsInRenderingThread());
		PrimitiveSceneProxy = InPrimitiveSceneProxy;
		
		ENQUEUE_RENDER_COMMAND(FVoxelChunkVertexFactory)([this, &InVertexBuffer](FRHICommandListImmediate& RHICmdList) {
			FDataType NewData;
			NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&InVertexBuffer, FVoxelVertex, X, VET_Float4);
			NewData.TextureCoordinates.Add(STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&InVertexBuffer, FVoxelVertex, X, VET_Float4));
			NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&InVertexBuffer, FVoxelVertex, X, VET_Color);
			NewData.TangentBasisComponents[0] = FVertexStreamComponent(&TangentBuffer, sizeof(FPackedNormal) * (2 * 0 + 0), 0, VET_PackedNormal);
			NewData.TangentBasisComponents[1] = FVertexStreamComponent(&TangentBuffer, sizeof(FPackedNormal) * (2 * 0 + 1), 0, VET_PackedNormal);
			this->SetData(NewData);
		});
	}

private:
	const FPrimitiveSceneProxy* PrimitiveSceneProxy;
};

/** Scene proxy */
class FVoxelChunkSceneProxy : public FPrimitiveSceneProxy
{
public:
	FVoxelChunkVertexBuffer VertexBuffer;
	FVoxelChunkIndexBuffer IndexBuffer;
	FVoxelChunkVertexFactory VertexFactory;

	FGraphEventRef SetupCompletionEvent;

	TUniformBufferRef<FLocalVertexFactoryUniformShaderParameters> UniformBuffer;

	FVoxelChunkSceneProxy(UVoxelRenderComponent* Component)
		: FPrimitiveSceneProxy(Component),
		VertexFactory(GetScene().GetFeatureLevel())
	{}

	virtual ~FVoxelChunkSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual uint32 GetMemoryFootprint(void) const { return(sizeof(*this) + GetAllocatedSize()); }
	SIZE_T GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }
	SIZE_T GetTypeHash() const override 
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	void BeginInitResources()
	{
		ENQUEUE_RENDER_COMMAND(FVoxelChunkSceneProxy)([=](FRHICommandListImmediate& RHICmdList) {
			FTaskGraphInterface::Get().WaitUntilTaskCompletes(SetupCompletionEvent, ENamedThreads::ActualRenderingThread);
		});
		BeginInitResource(&VertexBuffer);
		BeginInitResource(&IndexBuffer);
		VertexFactory.Init(VertexBuffer, this);
		BeginInitResource(&VertexFactory);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_BrickChunkSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		FMaterialRenderProxy* WireframeMaterialInstance = nullptr;
		if (bWireframe)
		{
			WireframeMaterialInstance = new FColoredMaterialRenderProxy(
				GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
				FLinearColor(0, 0.5f, 1.f)
			);

			Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);
		}

		// 暂时使用默认材质
		FMaterialRenderProxy* MaterialRenderProxy = bWireframe ? WireframeMaterialInstance : UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface)->GetRenderProxy();
		// 逐view 搜集
		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ++ViewIndex)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				// Create a mesh batch
				FMeshBatch& MeshBatch = Collector.AllocateMesh();
				MeshBatch.bUseAsOccluder = false;
				MeshBatch.VertexFactory = &VertexFactory;
				MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative();
				MeshBatch.DepthPriorityGroup = SDPG_World;
				MeshBatch.Type = EPrimitiveType::PT_TriangleList;
				MeshBatch.CastShadow = true;

				MeshBatch.bWireframe = bWireframe;
				MeshBatch.MaterialRenderProxy = MaterialRenderProxy;
				MeshBatch.bUseWireframeSelectionColoring = IsSelected() && bWireframe;

				// Setup index buffer
				FMeshBatchElement& Element = MeshBatch.Elements[0];
				Element.FirstIndex = 0;
				Element.NumPrimitives = IndexBuffer.Indices.Num() / 3;
				Element.IndexBuffer = &IndexBuffer;
				Element.MinVertexIndex = 0;
				Element.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;

				// Primitive uniform buffer is required.
				Element.PrimitiveUniformBuffer = GetUniformBuffer();
				MeshBatch.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, MeshBatch);
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;

		// 图元需要被画
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		// 有动态的元素
		Result.bDynamicRelevance = true;
		Result.bStaticRelevance = !Result.bDynamicRelevance;

		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
		Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;
		Result.bShadowRelevance = false;
		return Result;
	}

	virtual void OnTransformChanged() override
	{
	}

	virtual void CreateRenderThreadResources() override
	{
	}

	virtual void DestroyRenderThreadResources() override
	{

	}
};

UVoxelRenderComponent::UVoxelRenderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

FPrimitiveSceneProxy* UVoxelRenderComponent::CreateSceneProxy()
{
	FVoxelChunkSceneProxy* Proxy = new FVoxelChunkSceneProxy(this);
	Proxy->SetupCompletionEvent = FFunctionGraphTask::CreateAndDispatchWhenReady([Proxy]()
	{
		FVector4f Data[]
		{
			FVector4f{-50.0f, -50.0f, -50.0f, 1 }, FVector4f{50.0f, -50.0f, -50.0f, 1},
			FVector4f{50.0f,  50.0f, -50.0f, 1}, FVector4f{-50.0f,  50.0f, -50.0f, 1}
		};

		uint16 Index[] = {
			0, 2, 1,
			0, 3, 2
		};

		Proxy->VertexBuffer.Vertices.Append(Data);
		Proxy->IndexBuffer.Indices.Append(Index);
	});

	Proxy->BeginInitResources();
	return Proxy;
}

void UVoxelRenderComponent::GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const
{

}

FBoxSphereBounds UVoxelRenderComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = LocalToWorld.GetLocation();
	NewBounds.BoxExtent = FVector(1000.0f, 1000.0f, 1000.0f);
	NewBounds.SphereRadius = 1000.0f;
	return NewBounds;
}
