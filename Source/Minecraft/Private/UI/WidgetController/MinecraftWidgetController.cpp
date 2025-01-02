// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MinecraftWidgetController.h"
#include "Components/Inventory/BackpackComponent.h"

void UMinecraftWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	Pawn = WCParams.Pawn;
	Backpack = Pawn->GetComponentByClass<UBackpackComponent>();
}

void UMinecraftWidgetController::BroadcastInitialValue()
{
}

void UMinecraftWidgetController::BindCallbacksToDependencies()
{
}
