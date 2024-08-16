#pragma once

#include "CoreMinimal.h"
#include "Data/TerrainInfo.h"

class FLandscapeExtraTerrainEditor;

class FTerrainNoiseMapWorker : public FRunnable
{
public:
	FTerrainNoiseMapWorker(FLandscapeExtraTerrainEditor* InEditor, FTerrainNoiseInfo InNoiseInfo);
	~FTerrainNoiseMapWorker();

	// Begin FRunnable interface
	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	static FTerrainNoiseMapWorker* CreateNew(FLandscapeExtraTerrainEditor* InEditor, FTerrainNoiseInfo InNoiseInfo);

	static void ShutDown();

private:
	//float FBM(float InX, float InY, TArray<FVector2D> InOctaveOffsets, ETerrainFBMType InFBMType) const;
	float FBM(float InX, float InY, const TArray<FVector2D>& InOctaveOffsets, ETerrainFBMType InFBMType = ETerrainFBMType::OrdinaryFBM) const;

	void UpdateNoiseMap();

public:
	static FTerrainNoiseMapWorker* _Runnable;

private:
	FTerrainNoiseInfo NoiseInfo;

	FLandscapeExtraTerrainEditor* Editor = nullptr;

	TAtomic<bool> bRunThread;

	FRunnableThread* Thread = nullptr;
};
