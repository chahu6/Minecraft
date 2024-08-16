#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Data/TerrainInfo.h"

class UProceduralMeshComponent;

struct FMeshData
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	void Empty()
	{
		Vertices.Empty();
		Triangles.Empty();
		Normals.Empty();
		UV0.Empty();
		VertexColors.Empty();
		Tangents.Empty();
	}
};

class FTerrainMeshWorker : public FRunnable
{
public:
	FTerrainMeshWorker(UProceduralMeshComponent* InTerrainComponent, FTerrainMeshInfo InMeshInfo, FString InThreadName);
	//FTerrainMeshWorker(UProceduralMeshComponent* InTerrainComponent, FString InThreadName);
	virtual ~FTerrainMeshWorker();

	// Begin FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	// End Frunnable interface

	static FTerrainMeshWorker* CreateNew(UProceduralMeshComponent* InTerrainComponent, FTerrainMeshInfo InMeshInfo, FString InThreadName);
	//static FTerrainMeshWorker* CreateNew(UProceduralMeshComponent* InTerrainComponent, FString InThreadName);

	void ShutDown();

private:
	//float FBM(float InX, float InY, TArray<FVector2D> InOctaveOffsets, ETerrainFBMType InFBMType) const;
	//float FBM(float InX, float InY, TArray<FVector2D> InOctaveOffsets) const;

public:
	static FTerrainMeshWorker* _Runnable;

private:
	UProceduralMeshComponent* TerrainComponent = nullptr;

	//FTerrainMeshInfo MeshInfo;

	TAtomic<bool> bRunThread;

	FTerrainMeshInfo MeshInfo;

	FRunnableThread* Thread = nullptr;
};