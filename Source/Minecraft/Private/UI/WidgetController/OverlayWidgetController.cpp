// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "Player/EntityPlayer.h"
#include "Components/Inventory/BackpackComponent.h"

void UOverlayWidgetController::BroadcastInitialValue()
{

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (AEntityPlayer* Player = Cast<AEntityPlayer>(Pawn))
	{
		//Player->OnSwitchMainHand.AddLambda([this](int32 HotbarIndex) {
		//		OnSwitchMainHand.Broadcast(HotbarIndex);
		//	}
		//);
	}

	if (UBackpackComponent* BackpackComp = Pawn->GetComponentByClass<UBackpackComponent>())
	{
	/*	BackpackComp->OnHotbarItemUpdateDelegate.AddLambda([this](int32 Index, const FItemStack& NewItemStack) {
			OnHotbarUpdateSignature.Broadcast(Index, NewItemStack);
		});*/
	}
}
