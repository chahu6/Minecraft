#include "Math/PoissonDiscSampling.h"

void PoissonDiscSampling::GeneratePoints(TArray<FVector2D>& Points, float Radius, const FVector2D& SampleRegionSize, int32 NumSamples)
{
	float CellSize = Radius / FMath::Sqrt(2.f);

	int32 X = FMath::CeilToInt32(SampleRegionSize.X / CellSize);
	int32 Y = FMath::CeilToInt32(SampleRegionSize.Y / CellSize);

	TArray<TArray<int32>> Grid;
	Grid.SetNum(X);
	for (TArray<int32>& Elem : Grid)
	{
		Elem.SetNum(Y);
	}
	TArray<FVector2D> SpawnPoints;

	SpawnPoints.Add(SampleRegionSize / 2);
	while (!SpawnPoints.IsEmpty())
	{
		int32 SpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		FVector2D SpawnCentre = SpawnPoints[SpawnIndex];
		bool bCandidateAccepted = false;

		for (int32 i = 0; i < NumSamples; ++i)
		{
			float Angle = FMath::RandRange(0.f, 1.f) * 2 * PI;
			FVector2D Dir(FMath::Sin(Angle), FMath::Cos(Angle));
			FVector2D Candidate = SpawnCentre + Dir * FMath::RandRange(Radius, 2 * Radius);
			if (IsValid(Candidate, SampleRegionSize, CellSize, Radius, Points, Grid))
			{
				Points.Add(Candidate);
				SpawnPoints.Add(Candidate);
				Grid[(int32)(Candidate.X / CellSize)][(int32)(Candidate.Y / CellSize)] = Points.Num();
				bCandidateAccepted = true;
				break;
			}
		}
		if (!bCandidateAccepted)
		{
			SpawnPoints.RemoveAt(SpawnIndex);
		}
	}
}

bool PoissonDiscSampling::IsValid(const FVector2D& Candidate, const FVector2D& SampleRegionSize, float CellSize, float Radius, const TArray<FVector2D>& Points, const TArray<TArray<int32>>& Grid)
{
	if (Candidate.X >= 0 && Candidate.X < SampleRegionSize.X && Candidate.Y >= 0 && Candidate.Y < SampleRegionSize.Y)
	{
		int32 CellX = (int32)(Candidate.X / CellSize);
		int32 CellY = (int32)(Candidate.Y / CellSize);
		int32 SearchStartX = FMath::Max(0, CellX - 2);
		int32 SearchEndX = FMath::Min(CellX + 2, Grid.Num() - 1);
		int32 SearchStartY = FMath::Max(0, CellY - 2);
		int32 SearchEndY = FMath::Min(CellY + 2, Grid[0].Num() - 1);

		for (int32 X = SearchStartX; X <= SearchEndX; ++X)
		{
			for (int32 Y = SearchStartY; Y <= SearchEndY; ++Y)
			{
				int32 PointIndex = Grid[X][Y] - 1;
				if (PointIndex != -1)
				{
					float SqrDst = (Candidate - Points[PointIndex]).SquaredLength();
					if (SqrDst < Radius * Radius)
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	return false;
}
