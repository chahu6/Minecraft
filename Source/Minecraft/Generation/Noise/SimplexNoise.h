#pragma once

class SimplexNoise
{
public:
	SimplexNoise(const int32 Seed);

	double Noise2D(double X, double Y, double InFactor = 1.0F);
};