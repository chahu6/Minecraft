#include "LandscapeExtraTerrainEditorViewportClient.h"
#include "ProceduralMeshComponent.h"
#include "LandscapeExtraTerrain.h"
#include "Runnable/TerrainMeshWorker.h"
#include "Materials/MaterialInstance.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

FLandscapeExtraTerrainEditorViewportClient::FLandscapeExtraTerrainEditorViewportClient(ULandscapeExtraTerrain* InTerrain, FEditorModeTools* InModeTools, FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget)
	:FEditorViewportClient(InModeTools, InPreviewScene, InEditorViewportWidget),
	Terrain(InTerrain)
{
	DrawHelper.bDrawGrid = true;
	DrawHelper.bDrawPivot = true;
	SetRealtime(true);

	SetViewLocation(FVector(-400.f, 0.f, 200.f));
	SetViewRotation(FRotator(-45.0f, 0.0f, 0.0f));

	if (Terrain)
	{
		Terrain->OnTestChanged.BindRaw(this, &FLandscapeExtraTerrainEditorViewportClient::OnAssetChanged);
	}

	InitializerTerrain();
}

FLandscapeExtraTerrainEditorViewportClient::~FLandscapeExtraTerrainEditorViewportClient()
{
	if (TerrainMeshWorker)
	{
		TerrainMeshWorker->ShutDown();
		TerrainMeshWorker = nullptr;
	}
}

void FLandscapeExtraTerrainEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
}

bool FLandscapeExtraTerrainEditorViewportClient::InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
	return true;
}

void FLandscapeExtraTerrainEditorViewportClient::InitializerTerrain()
{
	TerrainComponent = NewObject<UProceduralMeshComponent>(GetTransientPackage(), TEXT("LandscapeExtraTerrain"));
	UMaterialInstance* Instance = LoadObject<UMaterialInstance>(NULL, TEXT("/Script/Engine.MaterialInstanceConstant'/LandscapeExtra/Material/Instance/MI_TerrainColor.MI_TerrainColor'"));
	if (Instance)
	{
		UMaterialInstanceDynamic* Tests = TerrainComponent->CreateDynamicMaterialInstance(0, Instance);
	}

	PreviewScene->AddComponent(TerrainComponent, FTransform(FRotator::ZeroRotator, FVector(-(Terrain->TerrainSize.X - 1) / 2.0f, -(Terrain->TerrainSize.Y - 1) / 2.0f, 0.0f), FVector(1.0f)));

	//SceneCaptureComponent
	SceneCaptureComponent = NewObject<USceneCaptureComponent2D>(GetTransientPackage(), TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComponent->OrthoWidth = 256;
	UTextureRenderTarget2D* Texture = LoadObject<UTextureRenderTarget2D>(NULL, TEXT("/Script/Engine.TextureRenderTarget2D'/LandscapeExtra/RT_TerrainNoiseMap.RT_TerrainNoiseMap'"));
	SceneCaptureComponent->TextureTarget = Texture;
	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;

	PreviewScene->AddComponent(SceneCaptureComponent, FTransform(FRotator(-90.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 200.0f), FVector(1.0f)));
}

void FLandscapeExtraTerrainEditorViewportClient::OnAssetChanged(float X, float Y)
{
	if (TerrainMeshWorker)
	{
		TerrainMeshWorker->ShutDown();
	}

	MeshInfo.Width = X;
	MeshInfo.Height = Y;
	MeshInfo.NoiseMap = Terrain->Data_Terrain_Default;

	TerrainMeshWorker = FTerrainMeshWorker::CreateNew(TerrainComponent, MeshInfo, TEXT("TerrainMeshWork"));
}
