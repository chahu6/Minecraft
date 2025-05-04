#include "BrickGridVertexFactory.h"

void FBrickGridVertexFactory::InitRHI()
{
	check(Streams.Num() == 0);

	FVertexDeclarationElementList Elements;
	// Position
	FVertexStreamComponent PosComponent;
	PosComponent.VertexBuffer = PositionVertexBuffer;
	PosComponent.Stride = sizeof(FVector4f);
	PosComponent.Offset = 0;
	PosComponent.VertexStreamUsage = EVertexStreamUsage::Default;
	PosComponent.Type = VET_Float4;
	Elements.Add(AccessStreamComponent(PosComponent, 0));

	// Tangent X
	FVertexStreamComponent TangentXComponent;
	TangentXComponent.VertexBuffer = TangentVertexBuffer;
	TangentXComponent.Stride = sizeof(FPackedNormal) * 2;
	TangentXComponent.Offset = 0;
	TangentXComponent.VertexStreamUsage = EVertexStreamUsage::Default;
	TangentXComponent.Type = VET_PackedNormal;
	Elements.Add(AccessStreamComponent(TangentXComponent, 1));

	// Tangent Z
	FVertexStreamComponent TangentZComponent;
	TangentZComponent.VertexBuffer = TangentVertexBuffer;
	TangentZComponent.Stride = sizeof(FPackedNormal) * 2;
	TangentZComponent.Offset = sizeof(FPackedNormal);
	TangentZComponent.VertexStreamUsage = EVertexStreamUsage::Default;
	TangentZComponent.Type = VET_PackedNormal;
	Elements.Add(AccessStreamComponent(TangentZComponent, 2));

	InitDeclaration(Elements, EVertexInputStreamType::Default);
	check(IsValidRef(GetDeclaration()));
}

void FBrickGridVertexFactory::ReleaseRHI()
{
	UniformBuffer.SafeRelease();
	FVertexFactory::ReleaseRHI();
}

void FBrickGridVertexFactory::SetParameters(const FCustomShaderVFParameters& InUniformParameters)
{
	UniformBuffer = TUniformBufferRef<FCustomShaderVFParameters>::CreateUniformBufferImmediate(InUniformParameters, EUniformBufferUsage::UniformBuffer_MultiFrame);
}

IMPLEMENT_VERTEX_FACTORY_TYPE(FBrickGridVertexFactory,
	"/CustomShaders/BrickGridVertexFactory.ush",
	EVertexFactoryFlags::UsedWithMaterials			 // 可用用于材质
	| EVertexFactoryFlags::SupportsManualVertexFetch // 支持Manual vertex fetch
);

void FMyMeshVFShaderParameters::Bind(const FShaderParameterMap& ParameterMap)
{
}

void FMyMeshVFShaderParameters::GetElementShaderBindings(const FSceneInterface* Scene, const FSceneView* View, const FMeshMaterialShader* Shader, const EVertexInputStreamType InputStreamType, ERHIFeatureLevel::Type FeatureLevel, const FVertexFactory* VertexFactory, const FMeshBatchElement& BatchElement, FMeshDrawSingleShaderBindings& ShaderBindings, FVertexInputStreamArray& VertexStreams) const
{
	FBrickGridVertexFactory* CustomShaderVF = (FBrickGridVertexFactory*)VertexFactory;
	const auto& ShaderParameter = Shader->GetUniformBufferParameter<FCustomShaderVFParameters>();
	ShaderBindings.Add(ShaderParameter, CustomShaderVF->GetUniformBuffer());
}

IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FCustomShaderVFParameters, "MyMeshVF");

IMPLEMENT_TYPE_LAYOUT(FMyMeshVFShaderParameters);
IMPLEMENT_VERTEX_FACTORY_PARAMETER_TYPE(FBrickGridVertexFactory, EShaderFrequency::SF_Vertex, FMyMeshVFShaderParameters);
