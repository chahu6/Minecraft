#include "LandscapeExtraEditorToolbar.h"
#include "LandscapeExtraTerrainEditor.h"
#include "WorkflowOrientedApp/SModeWidget.h"

#include "LandscapeExtraTerrainEditorApplicationModes.h"

FLandscapeExtraEditorToolbar::FLandscapeExtraEditorToolbar(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor)
	:LandscapeExtraTerrainEditor(InLandscapeExtraTerrainEditor)
{
}

void FLandscapeExtraEditorToolbar::AddCompileToolbar(UToolMenu* InMenu)
{
	FToolMenuSection& Section = InMenu->AddSection("Compile");
	Section.InsertPosition = FToolMenuInsert("Asset", EToolMenuInsertType::Before);
}

void FLandscapeExtraEditorToolbar::AddLandscapeExtraEditorModesToolbar(TSharedPtr<FExtender> Extender)
{
	TSharedPtr<FLandscapeExtraTerrainEditor> LandscapeEditorPtr = LandscapeExtraTerrainEditor.Pin();

	Extender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		LandscapeEditorPtr->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FLandscapeExtraEditorToolbar::FillLandscapeExtraEditorModesToolbar)
	);
}

void FLandscapeExtraEditorToolbar::FillLandscapeExtraEditorModesToolbar(FToolBarBuilder& ToolbarBuilder)
{
	TSharedPtr<FLandscapeExtraTerrainEditor> LandscapeEditorPtr = LandscapeExtraTerrainEditor.Pin();

	TAttribute<FName> GetActiveMode(LandscapeEditorPtr.ToSharedRef(), &FLandscapeExtraTerrainEditor::GetCurrentMode);
	FOnModeChangeRequested SetActiveMode = FOnModeChangeRequested::CreateSP(LandscapeEditorPtr.ToSharedRef(), &FLandscapeExtraTerrainEditor::SetCurrentMode);

	LandscapeEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));

	LandscapeEditorPtr->AddToolbarWidget(
		SNew(SModeWidget, FLandscapeExtraTerrainEditorApplicationModes::GetLocalizedMode(FLandscapeExtraTerrainEditorApplicationModes::NoiseMode), FLandscapeExtraTerrainEditorApplicationModes::NoiseMode)
		.OnGetActiveMode(GetActiveMode)
		.OnSetActiveMode(SetActiveMode)
		.CanBeSelected(LandscapeEditorPtr.Get(), &FLandscapeExtraTerrainEditor::IsEditingSingleAsset)
		.IconImage(FAppStyle::GetBrush("UMGEditor.SwitchToDesigner"))
	);

	LandscapeEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));

	LandscapeEditorPtr->AddToolbarWidget(
		SNew(SModeWidget, FLandscapeExtraTerrainEditorApplicationModes::GetLocalizedMode(FLandscapeExtraTerrainEditorApplicationModes::ErosionMode), FLandscapeExtraTerrainEditorApplicationModes::ErosionMode)
		.OnGetActiveMode(GetActiveMode)
		.OnSetActiveMode(SetActiveMode)
		.CanBeSelected(LandscapeEditorPtr.Get(), &FLandscapeExtraTerrainEditor::IsEditingSingleAsset)
		.IconImage(FAppStyle::GetBrush("UMGEditor.SwitchToDesigner"))
	);

	LandscapeEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));
}
