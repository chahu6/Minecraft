#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FLandscapeExtraTerrainEditorNoiseApplicationMode : public FApplicationMode
{
public:
	FLandscapeExtraTerrainEditorNoiseApplicationMode(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class FLandscapeExtraTerrainEditor> LandscapeExtraTerrainEditor;

	FWorkflowAllowedTabSet LandscapeExtraTerrainEditorTabFactories;
};











class FLandscapeExtraTerrainEditorErosionApplicationMode : public FApplicationMode
{
public:
	FLandscapeExtraTerrainEditorErosionApplicationMode(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override {};
	virtual void PostActivateMode() override {};

private:
	TWeakPtr<class FLandscapeExtraTerrainEditor> LandscapeExtraTerrainEditor;

	FWorkflowAllowedTabSet LandscapeExtraTerrainEditorTabFactories;
};


struct FLandscapeExtraTerrainEditorModes
{
	static const FName NoiseMode;

	static FText GetLocalizedMode(const FName InMode)
	{
		static TMap< FName, FText > LocModes;

		if (LocModes.Num() == 0)
		{
			LocModes.Add(NoiseMode, NSLOCTEXT("SomeModes", "SomeModeA", "Some Mode A"));
			LocModes.Add("LandscapeExtraTerrainEditorErosionApplicationMode", NSLOCTEXT("SomeModes", "SomeModeB", "Some Mode B"));
		}

		check(InMode != NAME_None);
		const FText* OutDesc = LocModes.Find(InMode);
		check(OutDesc);
		return *OutDesc;
	}
};