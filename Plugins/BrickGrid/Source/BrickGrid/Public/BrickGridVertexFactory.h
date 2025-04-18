// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VertexFactory.h"
#include "MeshMaterialShader.h"
#include "ShaderCore.h"
#include "DataDrivenShaderPlatformInfo.h"

class FVertexBuffer;

class FBrickGridVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FBrickGridVertexFactory);
public:
	FBrickGridVertexFactory(ERHIFeatureLevel::Type InFeatureLevel) 
		: FVertexFactory(InFeatureLevel), PositionVertexBuffer(nullptr), TangentVertexBuffer(nullptr) 
	{ }

	virtual void InitRHI() override;
	virtual void ReleaseRHI() override;

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
	FUniformBufferRHIRef UniformBuffer;
};