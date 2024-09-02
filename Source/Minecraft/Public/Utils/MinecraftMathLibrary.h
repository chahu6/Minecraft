// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinecraftMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	* ≤¥À…≈Ã≤…—˘
	*/
	UFUNCTION(BlueprintCallable, Category = "MinecraftMath")
	static void PoissonDiscSampling(/*UPARAM(ref) */TArray<FVector2D>& Points, float Radius, const FVector2D& SampleRegionSize, int32 NumSamples = 10);
};
