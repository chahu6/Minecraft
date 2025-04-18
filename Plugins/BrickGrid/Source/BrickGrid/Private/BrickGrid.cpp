// Copyright Epic Games, Inc. All Rights Reserved.

#include "BrickGrid.h"

#define LOCTEXT_NAMESPACE "FBrickGridModule"

void FBrickGridModule::StartupModule()
{
	const FString ShaderDirectory = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("BrickGrid/Shaders/Private"));
	AddShaderSourceDirectoryMapping("/CustomShaders", ShaderDirectory);
}

void FBrickGridModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBrickGridModule, BrickGrid)