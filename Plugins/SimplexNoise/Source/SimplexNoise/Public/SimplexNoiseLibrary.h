#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplexNoiseLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEXNOISE_API USimplexNoiseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static unsigned char perm[];
	static float _grad(int hash, float x);
	static float _grad(int hash, float x, float y);

	static float _simplexNoise2D(float x, float y);
	
public:
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static void SetNoiseSeed(const int32 newSeed);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoise2D(float x, float y, float inFactor = 1.0f);

	// 推荐inFactor参数值为[0.1, 0.00001]
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseInRange2D(float x, float y, float rangeMin, float rangeMax, float inFactor = 1.0f);
};
