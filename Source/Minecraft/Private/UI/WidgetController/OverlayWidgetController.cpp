// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "Player/MinecraftPlayer.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (AMinecraftPlayer* Player = PlayerController->GetPawn<AMinecraftPlayer>())
	{
		Player->OnSwitchMainHand.AddLambda(
			[this](int32 HotbarIndex)
			{
				OnSwitchMainHand.Broadcast(HotbarIndex);
			}
		);
	}
}
