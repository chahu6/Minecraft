#include "LandscapeExtraTerrainEditorViewport.h"
#include "LandscapeExtraTerrainEditorViewportClient.h"
#include "AdvancedPreviewScene.h"
#include "TerrainEditorViewportToolBar.h"

//#include "TestPreviewScene.h"

void SLandscapeExtraTerrainEditorViewport::Construct(const FArguments& InArgs)
{
	TerrainAsset = InArgs._TerrainAsset.Get();

	//PreviewScene = MakeShareable(new FPreviewScene());
	FPreviewScene::ConstructionValues ConstructionValues;
	PreviewScene = MakeShareable(new FAdvancedPreviewScene(ConstructionValues));
	PreviewScene->SetFloorVisibility(false);
	PreviewScene->SetEnvironmentVisibility(true);

	SEditorViewport::Construct(SEditorViewport::FArguments());
}

TSharedRef<class SEditorViewport> SLandscapeExtraTerrainEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SLandscapeExtraTerrainEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SLandscapeExtraTerrainEditorViewport::OnFloatingButtonClicked()
{
}

void SLandscapeExtraTerrainEditorViewport::OnFocusViewportToSelection()
{
	SEditorViewport::OnFocusViewportToSelection();
}

TSharedRef<FEditorViewportClient> SLandscapeExtraTerrainEditorViewport::MakeEditorViewportClient()
{
	if(!TerrainEditorViewportClient.IsValid())
		TerrainEditorViewportClient = MakeShareable(new FLandscapeExtraTerrainEditorViewportClient(TerrainAsset, nullptr, PreviewScene.Get()));

	return TerrainEditorViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SLandscapeExtraTerrainEditorViewport::MakeViewportToolbar()
{
	return SNew(STerrainEditorViewportToolBar, SharedThis(this));
}


