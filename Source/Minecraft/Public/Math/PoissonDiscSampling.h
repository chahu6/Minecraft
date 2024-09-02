#pragma once

/**
* ≤¥À…≈Ã≤…—˘
*/
class MINECRAFT_API PoissonDiscSampling
{
public:
	static void GeneratePoints(TArray<FVector2D>& Points, float Radius, const FVector2D& SampleRegionSize, int32 NumSamples = 15);

private:
	static bool IsValid(const FVector2D& Candidate, const FVector2D& SampleRegionSize, float CellSize, float Radius, const TArray<FVector2D>& Points, const TArray<TArray<int32>>& Grid);

private:
	PoissonDiscSampling() = delete;
};