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
	static double _grad(int hash, double x);
	static double _grad(int hash, double x, double y);

public:
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static void SetNoiseSeed(const int32 newSeed);

public:
	// 单型噪声
	static double _simplexNoise2D(double x, double y);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static double SimplexNoise2D(double x, double y, double inFactor = 1.0f);

	// 推荐inFactor参数值为[0.1, 0.00001]
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static double SimplexNoiseInRange2D(double x, double y, double rangeMin, double rangeMax, double inFactor = 1.0f);

public:
	// 柏林噪声
	static double _perlinNoise2D(double x, double y);

	static double Dot2D(int hash, double x, double y);

	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	static double PerlinNoise2D(double x, double y, double inFactor = 1.0f);

	// 推荐inFactor参数值为[0.1, 0.00001]
	UFUNCTION(BlueprintCallable, Category = "PerlinNoise")
	static double PerlinNoiseInRange2D(double x, double y, double rangeMin, double rangeMax, double inFactor = 1.0f);

public:
	// 改进柏林噪声
	static double _improvedNoise(double x, double y, double z);

	static double _grad(int hash, double x, double y, double z);

	// 比上面那个更好理解
	static double grad(int hash, double x, double y, double z);

	UFUNCTION(BlueprintCallable, Category = "ImprovedNoise")
	static double ImprovedNoise(double x, double y, double z, double inFactor = 1.0f);

public:
	static double FBM(int32 X, int32 Y, int32 Octaves = 5, double Lacunarity = 2.0, double Persistance = 0.5, double Factor = 0.0001);
private:
	inline static double Fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	inline static double Lerp(double t, double a, double b) { return a + t * (b - a); }
};
