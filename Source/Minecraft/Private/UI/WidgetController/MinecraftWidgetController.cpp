// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MinecraftWidgetController.h"

void UMinecraftWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	Pawn = WCParams.Pawn;
}

void UMinecraftWidgetController::BroadcastInitialValue()
{
}

void UMinecraftWidgetController::BindCallbacksToDependencies()
{
}
