// Fill out your copyright notice in the Description page of Project Settings.


#include "SwipeViewportClient.h"
#include "SwipeSettings.h"
#include "SwipeDelegates.h"
#include "Engine/UserInterfaceSettings.h"

bool USwipeViewportClient::InputTouch(FViewport* InViewport, int32 ControllerId, uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	if (IgnoreInput())
	{
		return false;
	}

	const USwipeSettings* SwipeSettings = GetDefault<USwipeSettings>();

	switch (Type)
	{
		case ETouchType::Began:
		{
			USwipeDelegates::TouchBeganDelegate.Broadcast(TouchLocation, Handle);

			SwipeStartLocation = TouchLocation;
			bSwiping = true;
			SwipeDirection = Swipe::Direction::None;

			break;
		}
		case ETouchType::Moved:
		{
			USwipeDelegates::TouchMovedDelegate.Broadcast(TouchLocation, Handle);

			float MinSwipeDistance = SwipeSettings->MinSwipeDistance;
			if (SwipeSettings->EnableDPIScaling)
			{
				MinSwipeDistance *= GetDPIScreenScale();
			}

			if (bSwiping && SwipeDirection == Swipe::Direction::None)
			{
				const FVector2D TouchDelta = TouchLocation - SwipeStartLocation;
				const float AbsX = FMath::Abs(TouchDelta.X);
				const float AbsY = FMath::Abs(TouchDelta.Y);
				const bool XMeetsThreshold = (AbsX >= MinSwipeDistance);
				const bool YMeetsThreshold = (AbsY >= MinSwipeDistance);

				if (AbsX > AbsY && XMeetsThreshold) 
				{
					SwipeTriggerLocation = TouchLocation;
					if (TouchDelta.X > 0) 
					{
						USwipeDelegates::SwipeRightDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation);
						SwipeDirection = Swipe::Direction::Right;
					}
					else
					{
						USwipeDelegates::SwipeLeftDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation);
						SwipeDirection = Swipe::Direction::Left;
					}
				}
				else if (YMeetsThreshold) 
				{
					SwipeTriggerLocation = TouchLocation;
					if (TouchDelta.Y > 0) 
					{
						USwipeDelegates::SwipeDownDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation);
						SwipeDirection = Swipe::Direction::Down;
					}
					else
					{
						USwipeDelegates::SwipeUpDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation);
						SwipeDirection = Swipe::Direction::Up;
					}
				}
			}
			break;
		}
		case ETouchType::Ended:
		{
			USwipeDelegates::TouchEndedDelegate.Broadcast(TouchLocation, Handle);

			bSwiping = false;

			switch (SwipeDirection)
			{
				case Swipe::Direction::Right:
				{
					USwipeDelegates::SwipeRightEndedDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation, TouchLocation);
					break;
				}
				case Swipe::Direction::Left:
				{
					USwipeDelegates::SwipeLeftEndedDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation, TouchLocation);
					break;
				}
				case Swipe::Direction::Down:
				{
					USwipeDelegates::SwipeDownEndedDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation, TouchLocation);
					break;
				}
				case Swipe::Direction::Up:
				{
					USwipeDelegates::SwipeUpEndedDelegate.Broadcast(SwipeStartLocation, SwipeTriggerLocation, TouchLocation);
					break;
				}
				default:
				{
					TapCount++;

					if (TapCount == 1)
					{
						USwipeDelegates::SingleTapDelegate.Broadcast(TouchLocation);
					}
					else if (TapCount >= 2)
					{
						USwipeDelegates::DoubleTapDelegate.Broadcast(TouchLocation);
					}

					FTimerHandle TapHandler;
					FTimerDelegate TapHandlerDelegate;
					TapHandlerDelegate.BindUObject(this, &USwipeViewportClient::ResetTapHandler);
					UWorld* MyWorld = GetWorld();
					if (MyWorld)
					{
						MyWorld->GetTimerManager().SetTimer(TapHandler, TapHandlerDelegate, SwipeSettings->MaxTimeBetweenTaps, false);
					}

					break;
				}
			}

			SwipeDirection = Swipe::Direction::None;
			break;
		}
		default:
			break;
	}

    return Super::InputTouch(InViewport, ControllerId, Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);
}

void USwipeViewportClient::ResetTapHandler()
{
	TapCount = 0;
}

float USwipeViewportClient::GetDPIScreenScale()
{
	if (GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
		return ViewportScale;
	}
	else
	{
		return 1.f;
	}
}
