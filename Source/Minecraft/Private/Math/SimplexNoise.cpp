#include "Math/SimplexNoise.h"
#include "SimplexNoiseLibrary.h"

int32 FSimplexNoises::Seed = 0;

FSimplexNoises::FSimplexNoises(double InFrequency, double InAmplitude, double InLacunarity, double InPersistance)
	:mFrequency(InFrequency),
	mAmplitude(InAmplitude),
	mLacunarity(InLacunarity),
	mPersistence(InPersistance)
{}

double FSimplexNoises::FBM(int32 Octaves, double X, double Y) const
{
	double NoiseSum = 0.0;
	double Denom = 0.0;
	double Frequency = mFrequency;
	double Amplitude = mAmplitude;

	for (int32 i = 0; i < Octaves; ++i)
	{
		NoiseSum += (Amplitude * USimplexNoiseLibrary::_simplexNoise2D(X * Frequency, Y * Frequency));
		Denom += Amplitude;

		Frequency *= mLacunarity;
		Amplitude *= mPersistence;
	}
	return (NoiseSum / Denom);
}

void FSimplexNoises::SetSeed(int32 NewSeed)
{
	Seed = NewSeed;
	USimplexNoiseLibrary::SetNoiseSeed(NewSeed);
}
