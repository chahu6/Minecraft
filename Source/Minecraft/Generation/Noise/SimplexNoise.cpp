#include "SimplexNoise.h"
#include "SimplexNoiseLibrary.h"

SimplexNoise::SimplexNoise(const int32 Seed)
{
	USimplexNoiseLibrary::SetNoiseSeed(Seed);
}

double SimplexNoise::Noise2D(double X, double Y, double InFactor)
{
	return USimplexNoiseLibrary::SimplexNoise2D(X, Y, InFactor);
}