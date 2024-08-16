#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FColorGradientSummoner : public FWorkflowTabFactory
{
public:
	FColorGradientSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor);

	//virtual FTabSpawnerEntry& RegisterTabSpawner(TSharedRef<FTabManager> TabManager, const FApplicationMode* CurrentApplicationMode) const;

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class FLandscapeExtraTerrainEditor> LandscapeExtraTerrainEditor;
};