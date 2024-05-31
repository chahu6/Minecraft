#include "Generation/Noise/PerlinSimplexNoise.h"
#include "Generation/Noise/SimplexNoise.h"

PerlinSimplexNoise::PerlinSimplexNoise(const int32 Seed, int32 Octave, const TArray<double>& InAmplitudes)
{
	FirstOctave = Octave;
	Amplitudes = InAmplitudes;
	int32 Number = Amplitudes.Num();

	for (int32 i = 0; i < Number; ++i)
	{
		if (Amplitudes[i] != 0.0f)
		{
			NoiseLevels.Add(MakeShared<SimplexNoise>(Seed));
		}
	}
	LowestFreqInputFactor = FMath::Pow(2.0f, FirstOctave);
	LowestFreqValueFactor = FMath::Pow(2.0f, (Number - 1)) / (FMath::Pow(2.0f, Number) - 1.0f);
}

TSharedPtr<PerlinSimplexNoise> PerlinSimplexNoise::Create(const int32 NewSeed, int32 Octave, const TArray<double>& InAmplitudes)
{
	return MakeShared<PerlinSimplexNoise>(NewSeed, Octave, InAmplitudes);
}

double PerlinSimplexNoise::GetValue(double X, double Y)
{
	double Result = 0.0f;
	double LoFreqInput = LowestFreqInputFactor;
	double LoFreqValue = LowestFreqValueFactor;
	for (int32 Octave = 0; Octave < NoiseLevels.Num(); ++Octave)
	{
		SimplexNoise* Noise = NoiseLevels[Octave].Get();
		if (Noise != nullptr)
		{
			double SourceNoise = Noise->Noise2D(LoFreqInput * X, LoFreqInput * Y);
			Result += Amplitudes[Octave] * SourceNoise * LoFreqValue;
		}
		LoFreqInput *= 2.0f;
		LoFreqValue /= 2.0f;
	}

	return Result;
}
