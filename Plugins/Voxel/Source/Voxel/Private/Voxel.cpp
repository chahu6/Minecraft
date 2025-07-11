// Copyright Epic Games, Inc. All Rights Reserved.

#include "Voxel.h"

#define LOCTEXT_NAMESPACE "FVoxelModule"

void FVoxelModule::StartupModule()
{
	const FString ShaderDirectory = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("Voxel/Shaders/Private"));
	AddShaderSourceDirectoryMapping("/CustomShader", ShaderDirectory);
}

void FVoxelModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoxelModule, Voxel)