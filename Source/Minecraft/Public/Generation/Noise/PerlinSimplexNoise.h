#pragma once
#include "CoreMinimal.h"

class SimplexNoise;

class PerlinSimplexNoise
{
public:
	PerlinSimplexNoise(const int32 Seed, int32 Octave, const TArray<double>& InAmplitudes);

public:
	static TSharedPtr<PerlinSimplexNoise> Create(const int32 NewSeed, int32 Octave, const TArray<double>& InAmplitudes);

	double GetValue(double X, double Y);
private:
	int32 FirstOctave;
	TArray<double> Amplitudes;
	double LowestFreqValueFactor;
	double LowestFreqInputFactor;
	TArray<TSharedPtr<SimplexNoise>> NoiseLevels;
};