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


	/** ������Button */
	void CreateButtonByStyle();
	void ButtonAction();

	/** ���� LayoutTab */
	void SpawnLayoutTab();
	TSharedRef<SDockTab> FOnSpawnMainTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> FOnSpawnSubTab(const FSpawnTabArgs& Args, FText Text);

	TSharedPtr<FTabManager> TabManager;
	TSharedPtr<FTabManager::FLayout> TabManagerLayout;
};
