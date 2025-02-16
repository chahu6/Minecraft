// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickChunkSceneProxy.h"
#include "BrickRenderComponent.h"
#include "DynamicMeshBuilder.h"
#include "Materials/MaterialRenderProxy.h"
#include "MaterialDomain.h"
#include "MeshMaterialShader.h"
#include "VoxelProcMeshBuffers.h"

FVoxelProcMeshBuffersRenderData::FVoxelProcMeshBuffersRenderData(const TSharedRef<const FVoxelProcMeshBuffers>& InBuffers, ERHIFeatureLevel::Type InFeatureLevel)
	:VertexFactory(InFeatureLevel, "FVoxelProcMeshBuffersRenderData")
	, Buffers(InBuffers)
{
	check(IsInRenderingThread());

	FVoxelProcMeshBuffers& InitBuffers = const_cast<FVoxelProcMeshBuffers&>(*Buffers);
	BeginInitResource(&InitBuffers.VertexBuffers.PositionVertexBuffer);
	BeginInitResource(&InitBuffers.VertexBuffers.StaticMeshVertexBuffer);
	BeginInitResource(&InitBuffers.VertexBuffers.ColorVertexBuffer);
	BeginInitResource(&InitBuffers.IndexBuffer);

	const FStaticMeshVertexBuffers& VertexBuffers = Buffers->VertexBuffers;
	const FDynamicMeshIndexBuffer32& IndexBuffer = Buffers->IndexBuffer;

	FLocalVertexFactory::FDataType Data;
	VertexBuffers.PositionVertexBuffer.BindPositionVertexBuffer(&VertexFactory, Data);
	VertexBuffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(&VertexFactory, Data);
	VertexBuffers.StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(&VertexFactory, Data);
	VertexBuffers.ColorVertexBuffer.BindColorVertexBuffer(&VertexFactory, Data);
	VertexFactory.SetData(Data);
	VertexFactory.InitResource();
}

FVoxelProcMeshBuffersRenderData::~FVoxelProcMeshBuffersRenderData()
{
	check(IsInRenderingThread());

	FVoxelProcMeshBuffers& InitBuffers = const_cast<FVoxelProcMeshBuffers&>(*Buffers);
	InitBuffers.VertexBuffers.PositionVertexBuffer.ReleaseResource();
	InitBuffers.VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
	InitBuffers.VertexBuffers.ColorVertexBuffer.ReleaseResource();
	InitBuffers.IndexBuffer.ReleaseResource();
	VertexFactory.ReleaseResource();
}

TSharedRef<FVoxelProcMeshBuffersRenderData> FVoxelProcMeshBuffersRenderData::GetRenderData(const TSharedRef<const FVoxelProcMeshBuffers>& InBuffers, ERHIFeatureLevel::Type InFeatureLevel)
{
	check(IsInRenderingThread());
	if (!InBuffers->RenderData.IsValid())
	{
		TSharedRef<FVoxelProcMeshBuffersRenderData> Result = TSharedRef<FVoxelProcMeshBuffersRenderData>(new FVoxelProcMeshBuffersRenderData(InBuffers, InFeatureLevel));
		InBuffers->RenderData = Result;
		return Result;
	}
	else
	{
		return InBuffers->RenderData.Pin().ToSharedRef();
	}
}

FBrickChunkSceneProxy::FBrickChunkSceneProxy(UBrickRenderComponent* InComponent)
	:FPrimitiveSceneProxy(InComponent)
	, Component(InComponent)
	, WireframeColor(InComponent->WireframeColor)
{
	//bWillEverBeLit = false; //设定为false则会跳过一些只被光照图元所需的工作

	//bHasDeformableMesh = false;

	//EnableGPUSceneSupportFlags();

	const int32 NumSections = Component->ProcMeshSections.Num();
	Sections.SetNum(NumSections);
	for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
	{
		const UBrickRenderComponent::FVoxelProcMeshSection& SrcSection = Component->ProcMeshSections[SectionIndex];
		FVoxelProcMeshProxySection& NewSection = Sections[SectionIndex];

		NewSection.Buffers = SrcSection.Buffers;
	}
}

FBrickChunkSceneProxy::~FBrickChunkSceneProxy()
{

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

	FMaterialRenderProxy* MaterialProxy = bWireframe ? WireframeMaterialInstance : UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface)->GetRenderProxy();
	for (const FVoxelProcMeshProxySection& Section : Sections)
	{
		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ++ViewIndex)
		{
			if (!(VisibilityMap & (1 << ViewIndex))) continue;

			FMeshBatch& Mesh = DrawSection(Collector, Section, MaterialProxy, false, bWireframe);
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

void FBrickChunkSceneProxy::CreateRenderThreadResources()
{
	check(IsInRenderingThread());

	for (FVoxelProcMeshProxySection& Section : Sections)
	{
		check(!Section.RenderData.IsValid());
		check(Section.Buffers.IsValid());
		Section.RenderData = FVoxelProcMeshBuffersRenderData::GetRenderData(Section.Buffers.ToSharedRef(), GetScene().GetFeatureLevel());
	}
}

void FBrickChunkSceneProxy::DestroyRenderThreadResources()
{
	check(IsInRenderingThread());
	
	for (FVoxelProcMeshProxySection& Section : Sections)
	{
		Section.RenderData.Reset();
	}
}

FMeshBatch& FBrickChunkSceneProxy::DrawSection(FMeshElementCollector& Collector, const FVoxelProcMeshProxySection& Section, const FMaterialRenderProxy* MaterialRenderProxy, bool bEnableTessellation, bool bWireframe) const
{
	// TODO: 在此处插入 return 语句
	check(Section.RenderData.IsValid());
	check(MaterialRenderProxy);

	FMeshBatch& Mesh = Collector.AllocateMesh();

	Mesh.VertexFactory = &Section.RenderData->VertexFactory;
	Mesh.bWireframe = bWireframe;
	Mesh.MaterialRenderProxy = MaterialRenderProxy;
	Mesh.bUseWireframeSelectionColoring = IsSelected() && bWireframe;
	// 是否反向剔除
	Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
	// 设定图元类型
	Mesh.Type = PT_TriangleList;
	// 设定深度优先级组
	Mesh.DepthPriorityGroup = SDPG_World;
	//
	Mesh.bCanApplyViewModeOverrides = false;

	/*bool bHasPrecomputedVolumetricLightmap;
	FMatrix PreviousLocalToWorld;
	int32 SingleCaptureIndex;
	bool bOutputVelocity;
	GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex, bOutputVelocity);
	bOutputVelocity |= AlwaysHasVelocity();

	FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
	DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, DrawsVelocity(), bOutputVelocity);*/

	FMeshBatchElement& BatchElement = Mesh.Elements[0];
	BatchElement.IndexBuffer = &Section.Buffers->IndexBuffer;
	//BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;
	//BatchElement.PrimitiveIdMode = PrimID_DynamicPrimitiveShaderData;

	// 设定索引范围
	BatchElement.FirstIndex = 0;
	BatchElement.NumPrimitives = Section.Buffers->IndexBuffer.Indices.Num() / 3;
	// 设定定点范围
	BatchElement.MinVertexIndex = 0;
	BatchElement.MaxVertexIndex = Section.Buffers->VertexBuffers.PositionVertexBuffer.GetNumVertices() - 1;
	
	return Mesh;
}
