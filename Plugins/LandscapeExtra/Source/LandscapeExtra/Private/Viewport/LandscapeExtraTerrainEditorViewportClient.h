#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "Data/TerrainInfo.h"

class ULandscapeExtraTerrain;
class UProceduralMeshComponent;
class FTerrainMeshWorker;

class FLandscapeExtraTerrainEditorViewportClient : public FEditorViewportClient
{
public:
	FLandscapeExtraTerrainEditorViewportClient(ULandscapeExtraTerrain* InTerrain, FEditorModeTools* InModeTools, FPreviewScene* InPreviewScene = nullptr, const TWeakPtr<SEditorViewport>& InEditorViewportWidget = nullptr);
	~FLandscapeExtraTerrainEditorViewportClient();
	//virtual void Draw(FViewport* InViewport, FCanvas* Canvas) override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual bool InputKey(FViewport* InViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.f, bool bGamepad = false) override;
	//virtual EMouseCursor::Type GetCursor(FViewport* InViewport, int32 X, int32 Y) override;
	
public:
	void InitializerTerrain();

private:
	void OnAssetChanged(float X, float Y);

private:
	ULandscapeExtraTerrain* Terrain = nullptr;

	UProceduralMeshComponent* TerrainComponent = nullptr;

	USceneCaptureComponent2D* SceneCaptureComponent = nullptr;

	FTerrainMeshInfo MeshInfo;

	FTerrainMeshWorker* TerrainMeshWorker = nullptr;
};