#pragma once

#include "CoreMinimal.h"

class MINECRAFT_API FSimplexNoises
{
public:
	FSimplexNoises(double InFrequency, double InAmplitude, double InLacunarity, double InPersistance);

	double FBM(int32 Octaves, double X, double Y) const;

    static int32 Seed;
    static void SetSeed(int32 NewSeed);

private:
    double mFrequency;   ///< Frequency ("width") of the first octave of noise (default to 1.0)
    double mAmplitude;   ///< Amplitude ("height") of the first octave of noise (default to 1.0)
    double mLacunarity;  ///< Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).
    double mPersistence; ///< Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)
};