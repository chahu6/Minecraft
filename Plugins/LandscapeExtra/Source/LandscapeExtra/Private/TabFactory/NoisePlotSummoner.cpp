#include "NoisePlotSummoner.h"
#include "LandscapeExtraTerrainEditor.h"
#include "LandscapeExtraTerrainEditorTabs.h"

#define LOCTEXT_NAMESPACE "FLandscapeExtraModule"

FNoisePlotSummoner::FNoisePlotSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:FWorkflowTabFactory(FLandscapeExtraTerrainEditorTabs::NoisePlotID, InLandscapeExtraTerrainEditor)
	, LandscapeExtraTerrainEditor(InLandscapeExtraTerrainEditor)
{
	TabLabel = LOCTEXT("NoisePlot", "NoisePlot");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Components");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("NewTabDescription", "NewTabDescription");
	ViewMenuTooltip = LOCTEXT("NoisePlotSummoner", "NoisePlotSummoner");
}

TSharedRef<SWidget> FNoisePlotSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return LandscapeExtraTerrainEditor.Pin()->SpawnNoisePlot();
}

FText FNoisePlotSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("NoisePlotSummoner", "The preview tab allows displaying models");
}

#undef LOCTEXT_NAMESPACE