// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroppableInventoryCellWidget.generated.h"

class UImage;
class UTextBlock;
struct FItemStack;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UDroppableInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FlushItemData(const FItemStack& ItemStack);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Quantity;
};
