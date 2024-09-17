#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

struct FMeshData
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	explicit FMeshData() = default;
};