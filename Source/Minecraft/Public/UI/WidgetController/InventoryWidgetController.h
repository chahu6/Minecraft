// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MinecraftWidgetController.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryHangItemUpdateSignature, const FItemStack&, NewItemStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdateSignature, int32, Index, const FItemStack&, NewItemStack);

struct FItemStack;
class USlot;

UENUM(BlueprintType)
enum class EMouseEvent : uint8
{
	LMB,
	RMB
};

/**
 * 
 */
UCLASS()
class MINECRAFT_API UInventoryWidgetController : public UMinecraftWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SlotClick(USlot* ClickedSlot, EMouseEvent MouseEvent);

	void MouseClick(int32 ClickedIndex, EMouseEvent MouseEvent);

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdateSignature OnInventoryUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryHangItemUpdateSignature OnInventoryHangItemUpdateDelegate;
};
