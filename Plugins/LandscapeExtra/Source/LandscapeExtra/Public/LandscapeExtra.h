// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetTypeActions_Terrain;

class FLandscapeExtraModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FAssetTypeActions_Terrain> TerrainActions;


	/** 创建新Button */
	void CreateButtonByStyle();
	void ButtonAction();

	/** 创建 LayoutTab */
	void SpawnLayoutTab();
	TSharedRef<SDockTab> FOnSpawnMainTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> FOnSpawnSubTab(const FSpawnTabArgs& Args, FText Text);

	TSharedPtr<FTabManager> TabManager;
	TSharedPtr<FTabManager::FLayout> TabManagerLayout;
};
