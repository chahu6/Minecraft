#include "TerrainMeshWorker.h"
#include "Async/Async.h"

FTerrainMeshWorker* FTerrainMeshWorker::_Runnable = nullptr;

FTerrainMeshWorker::FTerrainMeshWorker(UProceduralMeshComponent* InTerrainComponent, FTerrainMeshInfo InMeshInfo, FString InThreadName)
	:TerrainComponent(InTerrainComponent),
	MeshInfo(InMeshInfo),
	Thread(FRunnableThread::Create(this, *InThreadName))
{
}

FTerrainMeshWorker::~FTerrainMeshWorker()
{
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool FTerrainMeshWorker::Init()
{
	bRunThread.Store(true);

	return true;
}

uint32 FTerrainMeshWorker::Run()
{
	FMeshData MeshData;

	MeshData.Empty();

	for (int32 X = 0; X < MeshInfo.Width; ++X)
	{
		for (int32 Y = 0; Y < MeshInfo.Height; ++Y)
		{
			if (!bRunThread.Load()) return 0;

			MeshData.Vertices.Emplace(X, Y, MeshInfo.NoiseMap[Y + X * MeshInfo.Height]);
		}
	}

	for (int32 X = 0; X < MeshInfo.Width; ++X)
	{
		for (int32 Y = 0; Y < MeshInfo.Height; ++Y)
		{
			if (!bRunThread.Load()) return 0;

			if (MeshInfo.NoiseMap[Y + X * MeshInfo.Height] < 0.4f)
			{
				MeshData.VertexColors.Emplace(17.0f / 255, 62.0f / 255, 132.0f / 255, 1.0f);
			}
			else if (MeshInfo.NoiseMap[Y + X * MeshInfo.Height] <= 1.0f)
			{
				MeshData.VertexColors.Emplace(86.0f / 255, 151.0f / 255, 23.0f / 255, 1.0f);
			}
		}
	}

	for (int32 X = 0; X < MeshInfo.Width - 1; ++X)
	{
		for (int32 Y = 0; Y < MeshInfo.Height - 1; ++Y)
		{
			if (!bRunThread.Load()) return 0;

			int32 FirstIndex = MeshInfo.Height * X + Y;
			MeshData.Triangles.Add(FirstIndex);
			MeshData.Triangles.Add(FirstIndex + 1);
			MeshData.Triangles.Add(FirstIndex + MeshInfo.Height);
			MeshData.Triangles.Add(FirstIndex + MeshInfo.Height + 1);
			MeshData.Triangles.Add(FirstIndex + MeshInfo.Height);
			MeshData.Triangles.Add(FirstIndex + 1);
		}
	}

	if (bRunThread.Load())
	{
		UProceduralMeshComponent* TerrainComponent_Temp = TerrainComponent;
		AsyncTask(ENamedThreads::GameThread, [TerrainComponent_Temp, MeshData]()
		{
			if (TerrainComponent_Temp == nullptr) return;
			TerrainComponent_Temp->CreateMeshSection_LinearColor(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.VertexColors, MeshData.Tangents, true);
		});
	}

	return 0;
}

void FTerrainMeshWorker::Stop()
{
	bRunThread.Store(false);
}

void FTerrainMeshWorker::Exit()
{
}

FTerrainMeshWorker* FTerrainMeshWorker::CreateNew(UProceduralMeshComponent* InTerrainComponent, FTerrainMeshInfo InMeshInfo, FString InThreadName)
{
	if (!_Runnable && FPlatformProcess::SupportsMultithreading())
	{
		_Runnable = new FTerrainMeshWorker(InTerrainComponent, InMeshInfo, InThreadName);
	}

	return _Runnable;
}

void FTerrainMeshWorker::ShutDown()
{
	if (_Runnable)
	{
		_Runnable->Stop();
		_Runnable->Thread->WaitForCompletion();
		delete _Runnable;
		_Runnable = nullptr;
	}
}
