#include "LandscapeExtraTerrainEditorNoiseApplicationMode.h"
#include "LandscapeExtraTerrainEditor.h"
#include "LandscapeExtraTerrainEditorTabs.h"
#include "LandscapeExtraEditorToolbar.h"
#include "TabFactory/ViewportSummoner.h"
#include "TabFactory/DetailsTabSummoner.h"
#include "TabFactory/ColorGradientSummoner.h"
#include "TabFactory/NoiseMapSummoner.h"
#include "TabFactory/NoisePlotSummoner.h"

//#include "LandscapeExtraTerrainEditorApplicationModes.h"

const FName FLandscapeExtraTerrainEditorModes::NoiseMode(TEXT("LandscapeExtraTerrainEditorNoiseApplicationMode"));

FLandscapeExtraTerrainEditorNoiseApplicationMode::FLandscapeExtraTerrainEditorNoiseApplicationMode(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:FApplicationMode(FLandscapeExtraTerrainEditorModes::NoiseMode, FLandscapeExtraTerrainEditorModes::GetLocalizedMode)
{
	LandscapeExtraTerrainEditor = InLandscapeExtraTerrainEditor;

	LandscapeExtraTerrainEditorTabFactories.RegisterFactory(MakeShareable(new FDetailsTabSummoner(InLandscapeExtraTerrainEditor)));
	LandscapeExtraTerrainEditorTabFactories.RegisterFactory(MakeShareable(new FViewportSummoner(InLandscapeExtraTerrainEditor)));
	LandscapeExtraTerrainEditorTabFactories.RegisterFactory(MakeShareable(new FNoiseMapSummoner(InLandscapeExtraTerrainEditor)));
	LandscapeExtraTerrainEditorTabFactories.RegisterFactory(MakeShareable(new FNoisePlotSummoner(InLandscapeExtraTerrainEditor)));
	LandscapeExtraTerrainEditorTabFactories.RegisterFactory(MakeShareable(new FColorGradientSummoner(InLandscapeExtraTerrainEditor)));

	TabLayout = FTabManager::NewLayout("StardardStateMachineEditorLayout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		/*->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.4f)
			->SetHideTabWell(false)
			->AddTab(InLandscapeExtraTerrainEditor->GetToolbarTabId(), ETabState::OpenedTab)
		)*/
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(0.2f)
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.4f)
					->AddTab(FLandscapeExtraTerrainEditorTabs::NoiseMapID, ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(FLandscapeExtraTerrainEditorTabs::NoisePlotID, ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(FLandscapeExtraTerrainEditorTabs::ColorGradientID, ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f)
				->AddTab(FLandscapeExtraTerrainEditorTabs::PreviewViewpointID, ETabState::OpenedTab)
				->SetHideTabWell(false)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->AddTab(FLandscapeExtraTerrainEditorTabs::NoiseDetailsID, ETabState::OpenedTab)
				->SetHideTabWell(false)
			)
		)
	);

	InLandscapeExtraTerrainEditor->GetToolbarBuilder()->AddLandscapeExtraEditorModesToolbar(ToolbarExtender);
	//InLandscapeExtraTerrainEditor->RegisterModeToolbarIfUnregistered(GetModeName());

	//FName OutParentToolbarName;
	//FName ToolBarname = InLandscapeExtraTerrainEditor->GetToolMenuToolbarNameForMode(GetModeName(), OutParentToolbarName);
	//if (UToolMenu* Toolbar = UToolMenus::Get()->FindMenu(ToolBarname))
	//{
	//	//InLandscapeExtraTerrainEditor->GetToolbarBuilder()->AddCompileToolbar(Toolbar);
	//}
}

void FLandscapeExtraTerrainEditorNoiseApplicationMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	TSharedPtr<FLandscapeExtraTerrainEditor> Editor = LandscapeExtraTerrainEditor.Pin();

	Editor->RegisterToolbarTab(InTabManager.ToSharedRef());

	Editor->PushTabFactories(LandscapeExtraTerrainEditorTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FLandscapeExtraTerrainEditorNoiseApplicationMode::PreDeactivateMode()
{
}

void FLandscapeExtraTerrainEditorNoiseApplicationMode::PostActivateMode()
{

}


FLandscapeExtraTerrainEditorErosionApplicationMode::FLandscapeExtraTerrainEditorErosionApplicationMode(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:FApplicationMode(FName("LandscapeExtraTerrainEditorErosionApplicationMode"), FLandscapeExtraTerrainEditorModes::GetLocalizedMode)
{
	LandscapeExtraTerrainEditor = InLandscapeExtraTerrainEditor;

#if 0
	TabLayout = FTabManager::NewLayout("NewLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(0.2f)
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.4f)
					->AddTab("", ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab("", ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab("", ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.6f)
				->AddTab("", ETabState::OpenedTab)
				->SetHideTabWell(false)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->AddTab(FDetailsTabSummoner::TabID, ETabState::OpenedTab)
				->SetHideTabWell(false)
			)
		)
	);

	LandscapeExtraTerrainEditorTabFactories.RegisterFactory(MakeShareable(new FViewportSummoner(InLandscapeExtraTerrainEditor)));
#endif
}

void FLandscapeExtraTerrainEditorErosionApplicationMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	TSharedPtr<FLandscapeExtraTerrainEditor> Editor = LandscapeExtraTerrainEditor.Pin();

	Editor->RegisterTabSpawners(InTabManager.ToSharedRef());

	Editor->PushTabFactories(LandscapeExtraTerrainEditorTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}
