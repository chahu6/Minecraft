// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwipeDelegates.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class USwipeDelegates : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FTouchDelegate, FVector2D, int32);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FSwipeTriggeredDelegate, FVector2D, FVector2D);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FSwipeEndedDelegate, FVector2D, FVector2D, FVector2D);
	DECLARE_MULTICAST_DELEGATE_OneParam(FTapDelegate, FVector2D);

	static FTouchDelegate TouchBeganDelegate;
	static FTouchDelegate TouchMovedDelegate;
	static FTouchDelegate TouchEndedDelegate;

	static FSwipeTriggeredDelegate SwipeLeftDelegate;
	static FSwipeEndedDelegate SwipeLeftEndedDelegate;

	static FSwipeTriggeredDelegate SwipeRightDelegate;
	static FSwipeEndedDelegate SwipeRightEndedDelegate;

	static FSwipeTriggeredDelegate SwipeUpDelegate;
	static FSwipeEndedDelegate SwipeUpEndedDelegate;

	static FSwipeTriggeredDelegate SwipeDownDelegate;
	static FSwipeEndedDelegate SwipeDownEndedDelegate;

	static FTapDelegate SingleTapDelegate;
	static FTapDelegate DoubleTapDelegate;
};
