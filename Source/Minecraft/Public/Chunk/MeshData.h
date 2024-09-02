#pragma once

#include "CoreMinimal.h"
#include "MeshData.generated.h"

USTRUCT(BlueprintType)
struct FMeshData
{
	GENERATED_USTRUCT_BODY();

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;
};