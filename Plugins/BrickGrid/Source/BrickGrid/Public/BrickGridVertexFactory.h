// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VertexFactory.h"
#include "MeshMaterialShader.h"
#include "ShaderCore.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "MeshDrawShaderBindings.h"

class FVertexBuffer;

// Shader parameter
BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FCustomShaderVFParameters, )

	// UV描述信息不会全部用到，先占位。
	SHADER_PARAMETER(FIntVector4, VertexFetch_Parameters) //(ColorIndexMask, NumTexCoords, LightMapCoordIndex, EffectiveBaseVertexIndex)

	SHADER_PARAMETER_SRV(Buffer<float2>, VertexFetch_TexCoordBuffer)

END_GLOBAL_SHADER_PARAMETER_STRUCT()

class FBrickGridVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FBrickGridVertexFactory);
public:
	FBrickGridVertexFactory(ERHIFeatureLevel::Type InFeatureLevel) 
		: FVertexFactory(InFeatureLevel), PositionVertexBuffer(nullptr), TangentVertexBuffer(nullptr) 
	{ }

	virtual void InitRHI() override;
	virtual void ReleaseRHI() override;

	void SetParameters(const FCustomShaderVFParameters& InUniformParameters);

	FRHIUniformBuffer* GetUniformBuffer() const
	{
		return UniformBuffer.GetReference();
	}

	// 决定对应Shader是否编译
	static bool ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters) 
	{
		return RHISupportsManualVertexFetch(Parameters.Platform);
	}

	// 修改Shader编译参数
	static void ModifyCompilationEnvironment(const FVertexFactoryShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) 
	{
		if (RHISupportsManualVertexFetch(Parameters.Platform))
		{
			OutEnvironment.SetDefine(TEXT("MANUAL_VERTEX_FETCH"), TEXT("1"));
		}
	}

	// 验证Shader编译结果是否符合预期
	static void ValidateCompiledResult(const FVertexFactoryType* Type, EShaderPlatform Platform, const FShaderParameterMap& ParameterMap, TArray<FString>& OutErrors) 
	{
	}

	FVertexBuffer* PositionVertexBuffer;
	FVertexBuffer* TangentVertexBuffer;

private:
	FUniformBufferRHIRef UniformBuffer;
};

// Shader uniform buffer binding
class FMyMeshVFShaderParameters : public FVertexFactoryShaderParameters
{
	DECLARE_TYPE_LAYOUT(FMyMeshVFShaderParameters, NonVirtual);

public:
	void Bind(const FShaderParameterMap& ParameterMap);

	void GetElementShaderBindings(
		const FSceneInterface* Scene,
		const FSceneView* View,
		const FMeshMaterialShader* Shader,
		const EVertexInputStreamType InputStreamType,
		ERHIFeatureLevel::Type FeatureLevel,
		const FVertexFactory* VertexFactory,
		const FMeshBatchElement& BatchElement,
		FMeshDrawSingleShaderBindings& ShaderBindings,
		FVertexInputStreamArray& VertexStreams) const;

private:
};