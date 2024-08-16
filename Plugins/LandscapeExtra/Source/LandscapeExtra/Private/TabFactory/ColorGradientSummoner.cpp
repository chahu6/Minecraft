#include "ColorGradientSummoner.h"
#include "LandscapeExtraTerrainEditor.h"
#include "LandscapeExtraTerrainEditorTabs.h"

#define LOCTEXT_NAMESPACE "FLandscapeExtraModule"

FColorGradientSummoner::FColorGradientSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:FWorkflowTabFactory(FLandscapeExtraTerrainEditorTabs::ColorGradientID, InLandscapeExtraTerrainEditor)
	,LandscapeExtraTerrainEditor(InLandscapeExtraTerrainEditor)
{
	TabLabel = LOCTEXT("ColorGradient", "ColorGradient");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Components");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("NewTabDescription", "NewTabDescription");
	ViewMenuTooltip = LOCTEXT("ColorGradientTooltip", "ColorGradientTooltip");
}

TSharedRef<SWidget> FColorGradientSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return LandscapeExtraTerrainEditor.Pin()->SpawnColorGradient();
}

FText FColorGradientSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("ColorGradientTooltip", "The Test details tab allows editing of the properties");
}

#undef LOCTEXT_NAMESPACE