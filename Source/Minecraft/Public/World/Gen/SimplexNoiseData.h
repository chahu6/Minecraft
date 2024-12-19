// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimplexNoiseData.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API USimplexNoiseData : public UDataAsset
{
	GENERATED_BODY()

public:
	double FBM(int32 Octaves, double X, double Y) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	float mFrequency = 1.0f;   ///< Frequency ("width") of the first octave of noise (default to 1.0)

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	float mAmplitude = 1.0f;   ///< Amplitude ("height") of the first octave of noise (default to 1.0)

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	float mLacunarity = 2.0f;  ///< Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	float mPersistence = 0.5f; ///< Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)
};
