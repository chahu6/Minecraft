// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Gen/SimplexNoiseData.h"
#include "SimplexNoiseLibrary.h"

double USimplexNoiseData::FBM(int32 Octaves, double X, double Y) const
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
