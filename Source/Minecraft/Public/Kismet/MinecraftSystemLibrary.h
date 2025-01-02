// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinecraftSystemLibrary.generated.h"

class UBagWidgetController;
class AMinecraftHUD;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject"), Category = "MinecraftSystemLibrary|WidgetController")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AMinecraftHUD*& OutHUD);

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "WorldContextObject"), Category = "MinecraftSystemLibrary|WidgetController")
	static UBagWidgetController* GetBagWidgetController(const UObject* WorldContextObject);

};
