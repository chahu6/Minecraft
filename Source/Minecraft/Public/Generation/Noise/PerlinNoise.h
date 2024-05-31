#pragma once

#include "CoreMinimal.h"

class ImprovedNoise;

class PerlinNoise
{
public:
	PerlinNoise(const int32 Seed, int32 Octave, const TArray<double>& InAmplitudes);

public:
	static TSharedPtr<PerlinNoise> Create(const int32 NewSeed, int32 Octave, const TArray<double>& InAmplitudes);

	double GetValue(double X, double Y, double Z);
private:
	int32 FirstOctave;
	TArray<double> Amplitudes;
	double LowestFreqValueFactor;
	double LowestFreqInputFactor;
	TArray<TSharedPtr<ImprovedNoise>> NoiseLevels;
};