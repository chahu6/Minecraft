// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "Player/EntityPlayer.h"

void UOverlayWidgetController::BroadcastInitialValue()
{

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (AEntityPlayer* Player = Cast<AEntityPlayer>(Pawn))
	{
		Player->OnSwitchMainHand.AddLambda([this](int32 HotbarIndex) {
				OnSwitchMainHand.Broadcast(HotbarIndex);
			}
		);
	}
}
