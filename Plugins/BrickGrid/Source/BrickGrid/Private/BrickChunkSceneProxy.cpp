// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickChunkSceneProxy.h"
#include "BrickRenderComponent.h"
#include "DynamicMeshBuilder.h"
#include "Materials/MaterialRenderProxy.h"
#include "MaterialDomain.h"
#include "MeshMaterialShader.h"
#include "VoxelProcMeshBuffers.h"
#include "MeshDrawShaderBindings.h"

// Shader parameter
BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FCustomShaderVFParameters, )
SHADER_PARAMETER_SRV(Buffer<float2>, VertexFetch_TexCoordBuffer)
// UV描述信息不会全部用到，先占位。
SHADER_PARAMETER(FInt32Vector4, VertexFetch_Parameters) //(ColorIndexMask, NumTexCoords, LightMapCoordIndex, EffectiveBaseVertexIndex)
END_GLOBAL_SHADER_PARAMETER_STRUCT()

IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FCustomShaderVFParameters, "MyMeshVF");

// Shader uniform buffer binding
class FMyMeshVFShaderParameters : public FVertexFactoryShaderParameters 
{
	DECLARE_TYPE_LAYOUT(FMyMeshVFShaderParameters, NonVirtual);

public:
	void GetElementShaderBindings(
		const FSceneInterface* Scene,
		const FSceneView* View,
		const FMeshMaterialShader* Shader,
		const EVertexInputStreamType InputStreamType,
		ERHIFeatureLevel::Type FeatureLevel,
		const FVertexFactory* VertexFactory,
		const FMeshBatchElement& BatchElement,
		FMeshDrawSingleShaderBindings& ShaderBindings,
		FVertexInputStreamArray& VertexStreams) const
	{
		FBrickGridVertexFactory* CustomShaderVF = (FBrickGridVertexFactory*)VertexFactory;
		const auto& ShaderParameter = Shader->GetUniformBufferParameter<FCustomShaderVFParameters>();
		ShaderBindings.Add(ShaderParameter, CustomShaderVF->UniformBuffer);
	}
};

IMPLEMENT_TYPE_LAYOUT(FMyMeshVFShaderParameters);
IMPLEMENT_VERTEX_FACTORY_PARAMETER_TYPE(FBrickGridVertexFactory, EShaderFrequency::SF_Vertex, FMyMeshVFShaderParameters);


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
	, VertexFactory(GetScene().GetFeatureLevel())
{

	MyMaterialRenderProxy = InComponent->GetMyMaterial() ? InComponent->GetMyMaterial()->GetRenderProxy() : nullptr;

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

	FVector3f Data[]{
			{-50.0f, -50.0f, -50.0f}, { 50.0f, -50.0f, -50.0f}, { 50.0f,  50.0f, -50.0f}, { 50.0f,  50.0f, -50.0f}, {-50.0f,  50.0f, -50.0f}, {-50.0f, -50.0f, -50.0f},
			{-50.0f, -50.0f,  50.0f}, { 50.0f,  50.0f,  50.0f}, { 50.0f, -50.0f,  50.0f}, { 50.0f,  50.0f,  50.0f}, {-50.0f, -50.0f,  50.0f}, {-50.0f,  50.0f,  50.0f},
			{-50.0f,  50.0f,  50.0f}, {-50.0f, -50.0f, -50.0f}, {-50.0f,  50.0f, -50.0f}, {-50.0f, -50.0f, -50.0f}, {-50.0f,  50.0f,  50.0f}, {-50.0f, -50.0f,  50.0f},
			{ 50.0f,  50.0f,  50.0f}, { 50.0f,  50.0f, -50.0f}, { 50.0f, -50.0f, -50.0f}, { 50.0f, -50.0f, -50.0f}, { 50.0f, -50.0f,  50.0f}, { 50.0f,  50.0f,  50.0f},
			{-50.0f, -50.0f, -50.0f}, { 50.0f, -50.0f,  50.0f}, { 50.0f, -50.0f, -50.0f}, { 50.0f, -50.0f,  50.0f}, {-50.0f, -50.0f, -50.0f}, {-50.0f, -50.0f,  50.0f},
			{-50.0f,  50.0f, -50.0f}, { 50.0f,  50.0f, -50.0f}, { 50.0f,  50.0f,  50.0f}, { 50.0f,  50.0f,  50.0f}, {-50.0f,  50.0f,  50.0f}, {-50.0f,  50.0f, -50.0f},
	};

	uint16 Index[36];
	for (int32 i = 0; i < 36; ++i) {
		Index[i] = (uint16)i;
	}

	VertexBuffer.Vertices.Append(Data);
	IndexBuffer.Indices.Append(Index);
}

FBrickChunkSceneProxy::~FBrickChunkSceneProxy()
{
	VertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();
	VertexFactory.ReleaseResource();

	if (UVBuffer) UVBuffer->ReleaseResource();
	UVBufferSRV.SafeRelease();
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

	//FMaterialRenderProxy* MaterialProxy = bWireframe ? WireframeMaterialInstance : UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface)->GetRenderProxy();
	//for (const FVoxelProcMeshProxySection& Section : Sections)
	//{
	//	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ++ViewIndex)
	//	{
	//		if (!(VisibilityMap & (1 << ViewIndex))) continue;

	//		//FMeshBatch& Mesh = DrawSection(Collector, Section, MaterialProxy, false, bWireframe);
	//		//Collector.AddMesh(ViewIndex, Mesh);
	//	}
	//}

	// 暂时使用默认材质
	FMaterialRenderProxy* MaterialRenderProxy = bWireframe ? WireframeMaterialInstance : (MyMaterialRenderProxy ? MyMaterialRenderProxy : UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface)->GetRenderProxy());
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
	Result.bShadowRelevance = false;
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

	//for (FVoxelProcMeshProxySection& Section : Sections)
	//{
	//	check(!Section.RenderData.IsValid());
	//	check(Section.Buffers.IsValid());
	//	Section.RenderData = FVoxelProcMeshBuffersRenderData::GetRenderData(Section.Buffers.ToSharedRef(), GetScene().GetFeatureLevel());
	//}

	UVBuffer = MakeUnique<FVoxelChunkUVBuffer>();
	UVBuffer->InitResource();
	UVBufferSRV = RHICreateShaderResourceView(UVBuffer->VertexBufferRHI, sizeof(FVector2DHalf), PF_G16R16F);

	FCustomShaderVFParameters Params;
	Params.VertexFetch_Parameters = { 0, 1, 0, 0 };
	Params.VertexFetch_TexCoordBuffer = UVBufferSRV;
	VertexFactory.UniformBuffer = FCustomShaderVFParameters::CreateUniformBuffer(Params, EUniformBufferUsage::UniformBuffer_MultiFrame);

	VertexBuffer.InitResource();
	IndexBuffer.InitResource();
	VertexFactory.PositionVertexBuffer = &VertexBuffer;
	VertexFactory.InitResource();
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
