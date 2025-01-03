// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MinecraftWidgetController.h"
#include "UI/WidgetController/MouseEvent.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUpdateSignature, const FItemStack&, NewItemStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemClickedUpdateSignature, USlot*, ClickedSlot, const FItemStack&, NewItemStack);

struct FItemStack;
class USlot;
class UBackpackComponent;

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
	virtual void SlotClick(USlot* ClickedSlot, EMouseEvent MouseEvent);

	void MouseClick(int32 ClickedIndex, EMouseEvent MouseEvent);

	UPROPERTY(BlueprintAssignable)
	FOnItemClickedUpdateSignature OnSlotClickedUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdateSignature OnInventoryHangItemUpdateDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UBackpackComponent> BackpackComp;
};