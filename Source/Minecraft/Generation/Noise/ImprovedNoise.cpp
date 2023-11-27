#include "ImprovedNoise.h"
#include "SimplexNoiseLibrary.h"

ImprovedNoise::ImprovedNoise(const int32 Seed)
{
    USimplexNoiseLibrary::SetNoiseSeed(Seed);
}

double ImprovedNoise::Noise(double X, double Y, double Z, double InFactor)
{
    return USimplexNoiseLibrary::ImprovedNoise(X, Y, Z, InFactor);
}
