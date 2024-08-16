#include "LandscapeExtraTerrainEditor.h"
#include "Framework/Docking/TabManager.h"
#include "LandscapeExtraTerrainEditorNoiseApplicationMode.h"
#include "Viewport/LandscapeExtraTerrainEditorViewport.h"
#include "LandscapeExtraTerrainEditorApplicationModes.h"
#include "SlateMaterialBrush.h"
#include "LandscapeExtraTerrain.h"
#include "LandscapeExtraEditorToolbar.h"
#include "Runnable/TerrainNoiseMapWorker.h"
#include "Runnable/TerrainMeshWorker.h"
#include "Materials/MaterialInstance.h"
//#include "TerrainInfo.h"
//#include "Kismet/KismetRenderingLibrary.h"
//#include "Engine/Canvas.h"

void FLandscapeExtraTerrainEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FLandscapeExtraTerrainEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
}

void FLandscapeExtraTerrainEditor::InitializeAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, ULandscapeExtraTerrain* InAssets)
{
	//const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("NullLayout")->AddArea(FTabManager::NewPrimaryArea());
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FName("LandscapeExtraTerrainEditor"), FTabManager::FLayout::NullLayout, true, true, InAssets);

	TerrainAsset = InAssets;

	if (TerrainAsset.IsValid())
	{
		TerrainAsset->OnAssetPropertyChanged.BindRaw(this, &FLandscapeExtraTerrainEditor::OnAssetChanged);
	}

	if (!Toolbar.IsValid())
	{
		Toolbar = MakeShared<FLandscapeExtraEditorToolbar>(SharedThis(this));
	}

	CreateInternalWidgets();

	// Init Mode
	AddApplicationMode(FLandscapeExtraTerrainEditorApplicationModes::NoiseMode, MakeShareable(new FLandscapeExtraTerrainEditorNoiseApplicationMode(SharedThis(this))));
	//AddApplicationMode(FLandscapeExtraTerrainEditorApplicationModes::ErosionMode, MakeShareable(new FLandscapeExtraTerrainEditorErosionApplicationMode(SharedThis(this))));
	SetCurrentMode(FLandscapeExtraTerrainEditorApplicationModes::NoiseMode);
}

void FLandscapeExtraTerrainEditor::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

bool FLandscapeExtraTerrainEditor::IsEditingSingleAsset() const
{
	return GetAssetObj() != nullptr;
}

UObject* FLandscapeExtraTerrainEditor::GetAssetObj() const
{
	return GetEditingObjects().Num() == 1 ? GetEditingObjects()[0] : nullptr;
}

void FLandscapeExtraTerrainEditor::PostRegenerateMenusAndToolbars()
{
	RequestUpdateNoiseMap();
}

void FLandscapeExtraTerrainEditor::UpdateNoiseMap(float Width, float Height, const TArray<float>& NewNoiseMap)
{
	FScopeLock ScopeLock(&ThreadCritical);

	if (TerrainAsset.IsValid())
	{
		TerrainAsset->Data_Terrain_Default = NewNoiseMap;
		TerrainAsset->OnTestChanged.Execute(Width, Height);
	}
}

void FLandscapeExtraTerrainEditor::RequestUpdateNoiseMap()
{
	FTerrainNoiseInfo NewNoiseInfo;
	NewNoiseInfo.Seed = TerrainAsset->Seed;
	NewNoiseInfo.Width = TerrainAsset->TerrainSize.X; // !!!
	NewNoiseInfo.Height = TerrainAsset->TerrainSize.Y; // !!!
	NewNoiseInfo.Amplitude = TerrainAsset->Amplitude;
	NewNoiseInfo.Scale = TerrainAsset->Scale;
	NewNoiseInfo.Octaves = TerrainAsset->Octaves;
	NewNoiseInfo.Persistance = TerrainAsset->Persistance;
	NewNoiseInfo.Lacunarity = TerrainAsset->Lacunarity;
	NewNoiseInfo.Offset = TerrainAsset->Offset;
	NewNoiseInfo.BaseFBM = TerrainAsset->BaseFBM;
	NewNoiseInfo.WarpingFBM = TerrainAsset->WarpingFBM;
	NewNoiseInfo.WarpingStrength = TerrainAsset->WarpingStrength;
	NewNoiseInfo.bDomainWarping = TerrainAsset->bDomainWarping;
	NewNoiseInfo.HeightRemap = TerrainAsset->HeightRemap.GetRichCurve();
	NewNoiseInfo.EdgeType = TerrainAsset->EdgeType;
	NewNoiseInfo.Slope = TerrainAsset->Slope;
	NewNoiseInfo.bTerrace = TerrainAsset->bTerrace;
	NewNoiseInfo.StepSize = TerrainAsset->StepSize;
	NewNoiseInfo.NoiseMapRenderTarget = TerrainAsset->NoiseMapRenderTarget;
	NewNoiseInfo.bCanyons = TerrainAsset->bCanyons;
	NewNoiseInfo.Power = TerrainAsset->Power;

	FTerrainNoiseMapWorker::ShutDown();
	FTerrainNoiseMapWorker::CreateNew(this, NewNoiseInfo);
}

void FLandscapeExtraTerrainEditor::CreateInternalWidgets()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	//DetailsViewArgs.NotifyHook = this;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	//DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::ENameAreaSettings::HideNameArea;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(GetEditingObject());
	//DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FLandscapeExtraTerrainEditor::IsPropertyEditable));
	//DetailsView->OnFinishedChangingProperties().AddSP(this, &FLandscapeExtraTerrainEditor::OnFinishedChangingProperties);
}

TSharedRef<SWidget> FLandscapeExtraTerrainEditor::SpawnNoiseProperties()
{
	return
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SWidget> FLandscapeExtraTerrainEditor::SpawnPreview()
{
	return SNew(SLandscapeExtraTerrainEditorViewport).TerrainAsset(TerrainAsset.Get());
}

TSharedRef<SWidget> FLandscapeExtraTerrainEditor::SpawnNoiseMap()
{
	if (!NoiseMap.IsValid())
	{
		UMaterialInstance* MaterialInstance = LoadObject<UMaterialInstance>(GetTransientPackage(), TEXT("/Script/Engine.MaterialInstanceConstant'/LandscapeExtra/Material/Instance/MI_TerrainNoiseMap.MI_TerrainNoiseMap'"));
		NoiseMap = MakeShared<FSlateMaterialBrush>(*MaterialInstance, FVector2D(256));
	}

	return SNew(SImage).Image(NoiseMap.Get());
}

TSharedRef<SWidget> FLandscapeExtraTerrainEditor::SpawnColorGradient()
{
	/*
	check(TerrainAsset.Get());

		TSharedRef<SBorder> ColorGradientWidget =
		SNew(SBorder)
		.Padding(FMargin(4.0f, 4.0f, 4.0f, 0.0f))
		[
			SAssignNew(ColorGradientEditor, SColorGradientEditor)
				.ViewMinInput(0.0f)
				.ViewMaxInput(1.0f)
				.IsEditingEnabled(true)
		];

	FLandscapeExtraCurveOwner::FNotifyCurveChanged NewHandle;
	NewHandle.BindRaw(this, &FLandscapeExtraTerrainEditor::OnColorRemapChanged);
	CurveOwner.SetColorCurves(TerrainAsset->ColorRemap[0], TerrainAsset->ColorRemap[1], TerrainAsset->ColorRemap[2], TerrainAsset->ColorRemap[3], NAME_Color, *TerrainAsset, NewHandle);
	ColorGradientEditor.Get()->SetCurveOwner(&CurveOwner);

	return ColorGradientWidget;
	*/

	return SNew(SImage);
}

TSharedRef<SWidget> FLandscapeExtraTerrainEditor::SpawnNoisePlot()
{
	return SNew(SImage);
}

void FLandscapeExtraTerrainEditor::OnAssetChanged()
{
	RequestUpdateNoiseMap();
}

TSharedRef<SDockTab> FLandscapeExtraTerrainEditor::SpawnDetailTab(const FSpawnTabArgs& SpawnTabArgs) const
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	const FDetailsViewArgs DetailsViewArgs;
	const TSharedRef<IDetailsView> AssetPropertyView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	AssetPropertyView->SetObject(GetEditingObject());

	return SNew(SDockTab)
	[
		AssetPropertyView
	];
}
