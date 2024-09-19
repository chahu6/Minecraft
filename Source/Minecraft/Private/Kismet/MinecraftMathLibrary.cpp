// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/MinecraftMathLibrary.h"
#include "Math/PoissonDiscSampling.h"
#include "Utils/ChunkHelper.h"

void UMinecraftMathLibrary::PoissonDiscSampling(TArray<FVector2D>& Points, float Radius, const FVector2D& SampleRegionSize, int32 NumSamples)
{
	PoissonDiscSampling::GeneratePoints(Points, Radius, SampleRegionSize, NumSamples);
}

int32 UMinecraftMathLibrary::GetBlocksIndex(int32 OffsetX, int32 OffsetY, int32 OffsetZ)
{
	return ChunkHelper::GetBlocksIndex(OffsetX, OffsetY, OffsetZ);
}
