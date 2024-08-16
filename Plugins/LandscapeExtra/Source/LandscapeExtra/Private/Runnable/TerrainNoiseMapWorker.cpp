#include "TerrainNoiseMapWorker.h"
#include "Async/Async.h"
#include "LandscapeExtraTerrainEditor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

FTerrainNoiseMapWorker* FTerrainNoiseMapWorker::_Runnable = nullptr;

FTerrainNoiseMapWorker::FTerrainNoiseMapWorker(FLandscapeExtraTerrainEditor* InEditor, FTerrainNoiseInfo InNoiseInfo)
	: NoiseInfo(InNoiseInfo),
	  Editor(InEditor),
	  Thread(FRunnableThread::Create(this, TEXT("FTerrainNoiseMapWorker")))
{}

FTerrainNoiseMapWorker::~FTerrainNoiseMapWorker()
{
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool FTerrainNoiseMapWorker::Init()
{
	bRunThread.Store(true);
	return true;
}

uint32 FTerrainNoiseMapWorker::Run()
{
	float NoiseHeight;

	TArray<float> NewNoiseMap;
	const float Width = NoiseInfo.Width;
	const float Height = NoiseInfo.Height;
	NewNoiseMap.SetNum(Width * Height);

	float MaxNoiseHeight = std::numeric_limits<float>::min();
	float MinNoiseHeight = std::numeric_limits<float>::max();

	// Æ«ÒÆÒòËØ
	TArray<FVector2D> OctaveOffsets;
	OctaveOffsets.SetNum(NoiseInfo.Octaves);
	for (int32 i = 0; i < NoiseInfo.Octaves; ++i)
	{
		float OffsetX = FMath::RandRange(-100000.0, 100000.0) + NoiseInfo.Offset.X;
		float OffsetY = FMath::RandRange(-100000.0, 100000.0) + NoiseInfo.Offset.Y;
		OctaveOffsets[i] = FVector2D(OffsetX, OffsetY);
	}

	for (int32 X = 0; X < Width; ++X)
	{
		for (int32 Y = 0; Y < Height; ++Y)
		{
			if (!bRunThread.Load()) return 0;

			if (NoiseInfo.bDomainWarping)
			{
				const float FBM_x = FBM(X, Y, OctaveOffsets, NoiseInfo.WarpingFBM);
				const float FBM_y = FBM(X + 5.2f, Y + 1.3f, OctaveOffsets, NoiseInfo.WarpingFBM);

				NoiseHeight = FBM(X + FBM_x * NoiseInfo.WarpingStrength, Y + FBM_y * NoiseInfo.WarpingStrength, OctaveOffsets, NoiseInfo.BaseFBM);
			}
			else
			{
				NoiseHeight = FBM(X, Y, OctaveOffsets, NoiseInfo.BaseFBM);
			}

			if (NoiseHeight > MaxNoiseHeight)
			{
				MaxNoiseHeight = NoiseHeight;
			}
			if (NoiseHeight < MinNoiseHeight)
			{
				MinNoiseHeight = NoiseHeight;
			}

			NewNoiseMap[Y + X * Height] = NoiseHeight;
		}
	}

	// ¹æ·¶»¯Îª(0, 1);
	for (int32 X = 0; X < Width; ++X)
	{
		for (int32 Y = 0; Y < Height; ++Y)
		{
			NewNoiseMap[Y + X * Height] = UKismetMathLibrary::NormalizeToRange(NewNoiseMap[Y + X * Height], MinNoiseHeight, MaxNoiseHeight);
		}
	}

	// ÌÝÌï
	if (NoiseInfo.bTerrace)
	{
		float DividedHeight, StepX, StepY;
		StepX = NoiseInfo.StepSize.X;
		StepY = NoiseInfo.StepSize.Y;
		for (int32 i = 0; i < Width * Height; ++i)
		{
			DividedHeight = StepX == 0.0f ? 0.0f : NewNoiseMap[i] / StepX;
			switch (NoiseInfo.EdgeType)
			{
			case ETerraceEdgeType::Normal:
				NewNoiseMap[i] = NoiseInfo.Slope == 0.f ? 0.f : FMath::Floor(NewNoiseMap[i] * NoiseInfo.Slope) / NoiseInfo.Slope;
				break;
			case ETerraceEdgeType::Sharp:
				NewNoiseMap[i] = (FMath::Floor(DividedHeight) + FMath::Min(NoiseInfo.Slope * (DividedHeight - FMath::Floor(DividedHeight)), 1.0f)) * StepY;
				break;
			case ETerraceEdgeType::Smooth:
				NoiseInfo.Slope = FMath::Floor(NoiseInfo.Slope * 0.5f) * 2.0f + 1.0f;
				NewNoiseMap[i] = (FMath::RoundToFloat(DividedHeight) + 0.5f * FMath::Pow(2.0f * (DividedHeight - FMath::RoundToFloat(DividedHeight)), NoiseInfo.Slope)) * StepY;
				break;
			default:
				break;
			}
		}
	}

	// Ï¿¹È
	if (NoiseInfo.bCanyons)
	{
		for (int32 i = 0; i < Width * Height; ++i)
		{
			NewNoiseMap[i] = FMath::Pow(NewNoiseMap[i], NoiseInfo.Power);
		}
	}

	for (int32 i = 0; i < Width * Height; ++i)
	{
		if (!bRunThread.Load()) return 0;

		NewNoiseMap[i] = NoiseInfo.HeightRemap->Eval(NewNoiseMap[i]) * NoiseInfo.Amplitude;
	}

	UpdateNoiseMap();

	if (bRunThread.Load())
	{
		FLandscapeExtraTerrainEditor* EditorToUpdate = Editor;
		AsyncTask(ENamedThreads::GameThread, [EditorToUpdate, Width, Height, NewNoiseMap]()
		{
			EditorToUpdate->UpdateNoiseMap(Width, Height, NewNoiseMap);
		});
	}

	return 0;
}

float FTerrainNoiseMapWorker::FBM(float InX, float InY, const TArray<FVector2D>& InOctaveOffsets, ETerrainFBMType InFBMType) const
{
	float NoiseValue = 0.0f;
	float Amplitude = 1.0f;
	float Frequency = 1.0f;
	const float HalfWidth = NoiseInfo.Width * 0.5f;
	const float HalfHeight = NoiseInfo.Height * 0.5f;

	for (int32 i = 0; i < NoiseInfo.Octaves; ++i)
	{
		const float SampleX = (InX - HalfWidth + InOctaveOffsets[i].X) / NoiseInfo.Scale * Frequency;
		const float SampleY = (InY - HalfHeight + InOctaveOffsets[i].Y) / NoiseInfo.Scale * Frequency;
		float PerlinValue = FMath::PerlinNoise2D(FVector2D(SampleX, SampleY));

		switch (InFBMType)
		{
			case ETerrainFBMType::TurbulenceFBM:
			{
				NoiseValue += FMath::Abs(PerlinValue) * Amplitude;
				break;
			}
			case ETerrainFBMType::RidgeFBM:
			{
				PerlinValue = FMath::Abs(PerlinValue);
				PerlinValue = 1.0f - PerlinValue;
				PerlinValue = PerlinValue * PerlinValue;
				NoiseValue += PerlinValue * Amplitude;
				break;
			}
			default:
			{
				NoiseValue += PerlinValue * Amplitude;
				break;
			}
		}

		Amplitude *= NoiseInfo.Persistance;
		Frequency *= NoiseInfo.Lacunarity;
	}

	return NoiseValue;
}

void FTerrainNoiseMapWorker::UpdateNoiseMap()
{
	if (NoiseInfo.NoiseMapRenderTarget)
	{
		//UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(NoiseInfo.NoiseMapRenderTarget, NoiseInfo.NoiseMapRenderTarget)
	}
}

void FTerrainNoiseMapWorker::Stop()
{
	bRunThread.Store(false);
}

FTerrainNoiseMapWorker* FTerrainNoiseMapWorker::CreateNew(FLandscapeExtraTerrainEditor* InEditor, FTerrainNoiseInfo InNoiseInfo)
{
	if (!_Runnable && FPlatformProcess::SupportsMultithreading())
	{
		_Runnable = new FTerrainNoiseMapWorker(InEditor, InNoiseInfo);
	}

	return _Runnable;
}

void FTerrainNoiseMapWorker::ShutDown()
{
	if (_Runnable)
	{
		_Runnable->Stop();
		_Runnable->Thread->WaitForCompletion();
		delete _Runnable;
		_Runnable = nullptr;
	}
}
