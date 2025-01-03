// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/View.h"
#include "UI/WidgetController/MouseEvent.h"
#include "Container.generated.h"

class USlot;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UContainer : public UView
{
	GENERATED_BODY()
public:
	void SetActor(AActor* InOwnerActor);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SlotClick(USlot* ClickedSlot, EMouseEvent MouseEvent);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (ExposeOnSpawn = "true"))
	AActor* OwnerActor;
};
