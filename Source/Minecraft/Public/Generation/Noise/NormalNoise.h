#pragma once

#include "CoreMinimal.h"

class PerlinNoise;

class NormalNoise
{
public:
	NormalNoise(int32 FirstOctave, const TArray<double>& Amplitudes);

public:
	double GetValue(double X, double Y, double Z);

	static TSharedPtr<NormalNoise> Create(int32 Octave, const TArray<double>& InAmplitudes);
	static double ExpectedDeviation(double Value);
private:
	TSharedPtr<PerlinNoise> First;
	TSharedPtr<PerlinNoise> Second;

	double ValueFactor;
};