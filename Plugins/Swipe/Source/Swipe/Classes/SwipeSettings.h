// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwipeSettings.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class USwipeSettings : public UObject
{
	GENERATED_BODY()
public:
	// Minimum Swipe Distance
	UPROPERTY(Config, EditAnywhere, Category = General, meta = (DisplayName = "Minimum Swipe Distance"))
	float MinSwipeDistance = 50.f;

	// Maximum Time Between Taps To Register Double Tap
	UPROPERTY(Config, EditAnywhere, Category = General, meta = (DisplayName = "Maximum Time Between Taps"))
	float MaxTimeBetweenTaps = 0.3f;

	// Enable DPI Scaling to handle high density resolutions
	UPROPERTY(Config, EditAnywhere, Category = General, meta = (DisplayName = "Enable DPI Scaling"))
	bool EnableDPIScaling = true;
};
