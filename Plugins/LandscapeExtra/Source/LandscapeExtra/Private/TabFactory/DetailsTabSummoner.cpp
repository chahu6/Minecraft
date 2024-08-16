#include "DetailsTabSummoner.h"
#include "LandscapeExtraTerrainEditor.h"
#include "LandscapeExtraTerrainEditorTabs.h"

#define LOCTEXT_NAMESPACE "FLandscapeExtraModule"

FDetailsTabSummoner::FDetailsTabSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:FWorkflowTabFactory(FLandscapeExtraTerrainEditorTabs::NoiseDetailsID, InLandscapeExtraTerrainEditor)
	,LandscapeExtraTerrainEditor(InLandscapeExtraTerrainEditor)
{
	TabLabel = LOCTEXT("WidgetDetails_TabLabel", "Details");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("WidgetDetails_ViewMenu_Desc", "Details");
	ViewMenuTooltip = LOCTEXT("WidgetDetails_ViewMenu_ToolTip", "Show the Details");
}

TSharedRef<SWidget> FDetailsTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(LandscapeExtraTerrainEditor.IsValid());
	return LandscapeExtraTerrainEditor.Pin()->SpawnNoiseProperties();
}

FText FDetailsTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TestDetailsTabTooltip", "The Test details tab allows editing of the properties");
}

#undef LOCTEXT_NAMESPACE