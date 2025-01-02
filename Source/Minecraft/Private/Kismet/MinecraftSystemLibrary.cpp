// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/MinecraftSystemLibrary.h"
#include "UI/WidgetController/BagWidgetController.h"
#include "UI/HUD/MinecraftHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

bool UMinecraftSystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AMinecraftHUD*& OutHUD)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        OutHUD = PC->GetHUD<AMinecraftHUD>();
        if (OutHUD)
        {
            OutWCParams.PlayerController = PC;
            OutWCParams.Pawn = PC->GetPawn();
            OutWCParams.PlayerState = PC->GetPlayerState<APlayerState>();
            return true;
        }
    }
    return false;
}

UBagWidgetController* UMinecraftSystemLibrary::GetBagWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WCParams;
    AMinecraftHUD* MinecraftHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContextObject, WCParams, MinecraftHUD))
    {
        return MinecraftHUD->GetBagWidgetController(WCParams);
    }
    return nullptr;
}
