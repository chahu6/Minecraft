// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickChunkSceneProxy.h"
#include "BrickRenderComponent.h"
#include "DynamicMeshBuilder.h"
#include "Materials/MaterialRenderProxy.h"
#include "MaterialDomain.h"

static void InitVertexFactoryData(FBrickChunkVertexFactory* VertexFactory, FBrickChunkVertexBuffer* InVertexBuffer);

FBrickChunkSceneProxy::FBrickChunkSceneProxy(UBrickRenderComponent* InComponent)
	:FPrimitiveSceneProxy(InComponent)
	, VertexFactory(GetScene().GetFeatureLevel(), "FBrickGridSceneProxy")
	, WireframeColor(InComponent->WireframeColor)
{
	//bWillEverBeLit = false; //设定为false则会跳过一些只被光照图元所需的工作

#if 1
	TArray<FDynamicMeshVertex> OutVerts;
	FDynamicMeshVertex V0, V1, V2, V3;

	V0.Position = FVector3f(0.f, 0.f, 0.f);
	int32 I0 = OutVerts.Add(V0);
	V1.Position = FVector3f(100.f, 0.f, 0.f);
	int32 I1 = OutVerts.Add(V1);
	V2.Position = FVector3f(0.f, 100.f, 0.f);
	int32 I2 = OutVerts.Add(V2);
	V3.Position = FVector3f(100.f, 100.f, 0.f);
	int32 I3 = OutVerts.Add(V3);

	IndexBuffer.Indices.Add(I0);
	IndexBuffer.Indices.Add(I2);
	IndexBuffer.Indices.Add(I1);
	IndexBuffer.Indices.Add(I1);
	IndexBuffer.Indices.Add(I2);
	IndexBuffer.Indices.Add(I3);

	VertexBuffers.InitFromDynamicVertex(&VertexFactory, OutVerts);

	BeginInitResource(&VertexBuffers.StaticMeshVertexBuffer);
	BeginInitResource(&VertexBuffers.PositionVertexBuffer);
	BeginInitResource(&VertexBuffers.ColorVertexBuffer);

#else
	const int32 BoxSize = 100;

	VertexBuffer.Vertices.Add(FBrickVertex({ 0,0,0 }, 0));
	VertexBuffer.Vertices.Add(FBrickVertex({ BoxSize,0,0 }, 0));
	VertexBuffer.Vertices.Add(FBrickVertex({ 0,BoxSize,0 }, 0));
	VertexBuffer.Vertices.Add(FBrickVertex({ 0,0,BoxSize }, 0));

	//填充索引
	IndexBuffer.Indices.Add(0);
	IndexBuffer.Indices.Add(1);
	IndexBuffer.Indices.Add(2);
	IndexBuffer.Indices.Add(0);
	IndexBuffer.Indices.Add(2);
	IndexBuffer.Indices.Add(3);
	IndexBuffer.Indices.Add(0);
	IndexBuffer.Indices.Add(3);
	IndexBuffer.Indices.Add(1);
	IndexBuffer.Indices.Add(3);
	IndexBuffer.Indices.Add(2);
	IndexBuffer.Indices.Add(1);

	//VertexFactory.Init(&VertexBuffer);

	InitVertexFactoryData(&VertexFactory, &VertexBuffer);

	BeginInitResource(&VertexBuffer);
#endif

	BeginInitResource(&VertexFactory);
	BeginInitResource(&IndexBuffer);
}

FBrickChunkSceneProxy::~FBrickChunkSceneProxy()
{
	//VertexBuffer.ReleaseResource();
	VertexBuffers.PositionVertexBuffer.ReleaseResource();
	VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
	VertexBuffers.ColorVertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();
	VertexFactory.ReleaseResource();
}

SIZE_T FBrickChunkSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}

void FBrickChunkSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_BrickChunkSceneProxy_GetDynamicMeshElements);

	const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

	FMaterialRenderProxy* WireframeMaterialInstance = nullptr;
	if (bWireframe)
	{
		WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
			WireframeColor
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);
	}

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ++ViewIndex)
	{
		if (VisibilityMap & (1 << ViewIndex))
		{
			FMaterialRenderProxy* MaterialProxy = bWireframe ? WireframeMaterialInstance : UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface)->GetRenderProxy();

			FMeshBatch& Mesh = Collector.AllocateMesh();
			FMeshBatchElement& BatchElement = Mesh.Elements[0];
			BatchElement.IndexBuffer = &IndexBuffer;
			Mesh.VertexFactory = &VertexFactory;
			Mesh.bWireframe = bWireframe;
			Mesh.MaterialRenderProxy = MaterialProxy;

			bool bHasPrecomputedVolumetricLightmap;
			FMatrix PreviousLocalToWorld;
			int32 SingleCaptureIndex;
			bool bOutputVelocity;
			GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex, bOutputVelocity);
			bOutputVelocity |= AlwaysHasVelocity();

			FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
			DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, DrawsVelocity(), bOutputVelocity);
			BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

			// 设定索引范围
			BatchElement.FirstIndex = 0;
			BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
			// 设定定点范围
			BatchElement.MinVertexIndex = 0;
			//BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
			BatchElement.MaxVertexIndex = VertexBuffers.PositionVertexBuffer.GetNumVertices() - 1;

			// 是否反向剔除
			Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
			// 设定图元类型
			Mesh.Type = PT_TriangleList;
			// 设定深度优先级组
			Mesh.DepthPriorityGroup = SDPG_World;
			//
			Mesh.bCanApplyViewModeOverrides = false;

			Collector.AddMesh(ViewIndex, Mesh);
		}
	}
}

FPrimitiveViewRelevance FBrickChunkSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;

	// 图元需要被画
	Result.bDrawRelevance = IsShown(View);
	Result.bShadowRelevance = IsShadowCast(View);
	// 有动态的元素
	Result.bDynamicRelevance = true;

	Result.bRenderInMainPass = ShouldRenderInMainPass();
	Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
	Result.bRenderCustomDepth = ShouldRenderCustomDepth();
	Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
	Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;

	return Result;
}

void FBrickChunkSceneProxy::OnTransformChanged()
{
	Origin = GetLocalToWorld().GetOrigin();
}

void FBrickChunkSceneProxy::BuildFace(const FVector& InBlockPos)
{
	//const int32 Index = 
}

/* Helper function that initializes a render resource if it's not initialized, or updates it otherwise*/
static inline void InitOrUpdateResource(FRenderResource* Resource)
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

static void InitVertexFactoryData(FBrickChunkVertexFactory* VertexFactory, FBrickChunkVertexBuffer* InVertexBuffer)
{
	ENQUEUE_RENDER_COMMAND(StaticMeshVertexBuffersLegacyInit)(
		[VertexFactory, InVertexBuffer](FRHICommandListImmediate& RHICmdList)
		{
			//Initialize or update the RHI vertex buffers
			InitOrUpdateResource(InVertexBuffer);

			//Use the RHI vertex buffers to create the needed Vertex stream components in an FDataType instance, and then set it as the data of the vertex factory
			FLocalVertexFactory::FDataType Data;

			Data.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_UByte4N);
			Data.TextureCoordinates.Add(STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_UByte4N));
			Data.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(InVertexBuffer, FBrickVertex, X, VET_Color);

			VertexFactory->SetData(Data);

			//Initalize the vertex factory using the data that we just set, this will call the InitRHI() method that we implemented in out vertex factory
			InitOrUpdateResource(VertexFactory);
		});
}