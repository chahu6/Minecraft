#pragma once

#include "CoreMinimal.h"

class ImprovedNoise
{
public:
	ImprovedNoise(const int32 Seed);

	double Noise(double X, double Y, double Z, double InFactor = 1.0F);
};