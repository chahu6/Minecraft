#include "NoiseMapSummoner.h"
#include "LandscapeExtraTerrainEditor.h"
#include "LandscapeExtraTerrainEditorTabs.h"

#define LOCTEXT_NAMESPACE "FLandscapeExtraModule"

FNoiseMapSummoner::FNoiseMapSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:FWorkflowTabFactory(FLandscapeExtraTerrainEditorTabs::NoiseMapID, InLandscapeExtraTerrainEditor)
	,LandscapeExtraTerrainEditor(InLandscapeExtraTerrainEditor)
{
	TabLabel = LOCTEXT("NoiseMap", "NoiseMap");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Components");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("NewTabDescription", "NewTabDescription");
	ViewMenuTooltip = LOCTEXT("NoiseMapSummoner", "NoiseMapSummoner");
}

TSharedRef<SWidget> FNoiseMapSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return LandscapeExtraTerrainEditor.Pin()->SpawnNoiseMap();
}

FText FNoiseMapSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("NoiseMapSummoner", "The preview tab allows displaying models");
}

#undef LOCTEXT_NAMESPACE