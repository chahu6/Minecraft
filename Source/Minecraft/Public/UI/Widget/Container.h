// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Container.generated.h"

class AEntityPlayer;
class IInventoryInterface;
class UInventoryItem;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UContainer : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void OnContainerClosed(AEntityPlayer* PlayerIn);

protected:
	virtual void NativePreConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual void NativeDestruct() override;

	void ClearContainer(AEntityPlayer* PlayerIn, IInventoryInterface* InventoryIn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Container")
	void HandleLMB(UInventoryItem* InventoryItem);
	virtual void HandleLMB_Implementation(UInventoryItem* InventoryItem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Container")
	void HandleRMB(UInventoryItem* InventoryItem);
	virtual void HandleRMB_Implementation(UInventoryItem* InventoryItem);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	TObjectPtr<AEntityPlayer> Player;
};
