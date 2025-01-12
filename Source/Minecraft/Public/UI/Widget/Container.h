// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/View.h"
#include "UI/WidgetController/MouseEvent.h"
#include "Container.generated.h"

class USlot;
class UCanvasPanel;
class UDroppableInventoryCellWidget;

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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SlotClick(USlot* ClickedSlot, EMouseEvent MouseEvent);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (ExposeOnSpawn = "true"))
	AActor* OwnerActor;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	TObjectPtr<UDroppableInventoryCellWidget> HangItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	FVector2D HangImageSize = { 100.f, 100.f };
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
};
