// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MinecraftWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, int32, Index);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MINECRAFT_API UOverlayWidgetController : public UMinecraftWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "WidgetController|Attributes")
	FOnAttributeChangedSignature OnSwitchMainHand;
};
