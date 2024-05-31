#include "Generation/Noise/NormalNoise.h"
#include "Generation/Noise/PerlinNoise.h"

NormalNoise::NormalNoise(int32 FirstOctave, const TArray<double>& Amplitudes)
{
	First = PerlinNoise::Create(FMath::Rand(), FirstOctave, Amplitudes);
	Second = PerlinNoise::Create(FMath::Rand(), FirstOctave, Amplitudes);

	int32 MinOctave = INT_MAX;
	int32 MaxOctave = INT_MIN;
	for (auto Itr = Amplitudes.CreateConstIterator(); Itr; ++Itr)
	{
		int32 Index = Itr.GetIndex();
		double Amplitude = *Itr;
		if (Amplitude == 0.0) continue;
		MinOctave = FMath::Min(MinOctave, Index);
		MaxOctave = FMath::Max(MaxOctave, Index);
	}
	ValueFactor = 0.16666666666666666 / ExpectedDeviation(MaxOctave - MinOctave);
}

double NormalNoise::GetValue(double X, double Y, double Z)
{
	double dx = X * 1.0181268882175227;
	double dy = Y * 1.0181268882175227;
	double dz = Z * 1.0181268882175227;
	return (First->GetValue(X, Y, Z) + Second->GetValue(dx, dy, dz)) * ValueFactor;
}

TSharedPtr<NormalNoise> NormalNoise::Create(int32 Octave, const TArray<double>& InAmplitudes)
{
	return MakeShared<NormalNoise>(Octave, InAmplitudes);
}

double NormalNoise::ExpectedDeviation(double Value)
{
	return 0.1f * (1.0f + 1.0f / (Value + 1));
}
