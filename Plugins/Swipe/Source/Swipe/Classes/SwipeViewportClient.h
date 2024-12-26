// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "SwipeViewportClient.generated.h"

UENUM(BlueprintType)
namespace Swipe {
	enum Direction {
		None,
		Left,
		Right,
		Up,
		Down
	};
}

/**
 * 
 */
UCLASS()
class USwipeViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
public:
	virtual bool InputTouch(FViewport* Viewport, int32 ControllerId, uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;

protected:
	bool bSwiping;
	Swipe::Direction SwipeDirection;
	FVector2D SwipeStartLocation;
	FVector2D SwipeTriggerLocation;
	int32 TapCount = 0;

	UFUNCTION()
	void ResetTapHandler();

	float GetDPIScreenScale();
};
