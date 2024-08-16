#include "ViewportSummoner.h"
#include "LandscapeExtraTerrainEditor.h"
#include "LandscapeExtraTerrainEditorTabs.h"

#define LOCTEXT_NAMESPACE "FLandscapeExtraModule"

FViewportSummoner::FViewportSummoner(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditorPtr)
	: FWorkflowTabFactory(FLandscapeExtraTerrainEditorTabs::PreviewViewpointID, InLandscapeExtraTerrainEditorPtr)
	, LandscapeExtraTerrainEditorPtr(InLandscapeExtraTerrainEditorPtr)
{
	TabLabel = LOCTEXT("PreviewViewport", "PreviewViewport");
	TabIcon = FSlateIcon (FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Components");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("NewTabDescription", "NewTabDescription");
	ViewMenuTooltip = LOCTEXT("TestPreviewTabTooltip", "TestPreviewTabTooltip");
}

TSharedRef<SWidget> FViewportSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return LandscapeExtraTerrainEditorPtr.Pin()->SpawnPreview();
}

FText FViewportSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TestPreviewTabTooltip", "The preview tab allows displaying models");
}

#undef LOCTEXT_NAMESPACE