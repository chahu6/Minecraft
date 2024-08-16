#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FViewportSummoner : public FWorkflowTabFactory
{
//public:
//	static const FName TabID;
public:
	FViewportSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditorPtr);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class FLandscapeExtraTerrainEditor> LandscapeExtraTerrainEditorPtr;
};