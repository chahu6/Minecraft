// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MinecraftMathLibrary.h"
#include "Math/PoissonDiscSampling.h"

void UMinecraftMathLibrary::PoissonDiscSampling(TArray<FVector2D>& Points, float Radius, const FVector2D& SampleRegionSize, int32 NumSamples)
{
	PoissonDiscSampling::GeneratePoints(Points, Radius, SampleRegionSize, NumSamples);
}
