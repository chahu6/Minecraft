// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/View.h"
#include "UI/MouseEvent.h"
#include "Container.generated.h"

class USlot;
class UCanvasPanel;
class UDroppableInventoryCellWidget;
class UBackpackComponent;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UContainer : public UView
{
	GENERATED_BODY()
public:
	UContainer();
	void SetActor(AActor* InOwnerActor);

	UFUNCTION(BlueprintCallable, Category = "View", meta = (ComponentClass = "/Script/Engine.ActorComponent"), meta = (DeterminesOutputType = "ComponentClass"))
	UActorComponent* GetComponentByClass(TSubclassOf<UActorComponent> ComponentClass) const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BindCallbacksToDependencies();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SlotClick(USlot* ClickedSlot, EMouseEvent MouseEvent);

	UFUNCTION(BlueprintNativeEvent)
	void FlushHangItem(const FItemStack& NewItemStack);

	UFUNCTION(BlueprintCallable)
	void FlushItemFromWidget(UPanelWidget* PanelWidget, int32 Index, const FItemStack& NewItemStack);

	UFUNCTION()
	void OnBagItemUpdateDelegateEvent(int32 Index, const FItemStack& NewItemStack);

	UFUNCTION()
	void OnHotbarItemUpdateDelegateEvent(int32 Index, const FItemStack& NewItemStack);

	UFUNCTION()
	void OnHangItemUpdateEvent(const FItemStack& NewItemStack);

	void InitBackpack();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (ExposeOnSpawn = "true"))
	AActor* OwnerActor;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	TObjectPtr<UDroppableInventoryCellWidget> HangItem;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<UDroppableInventoryCellWidget> HangItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<USlot> SlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	FVector2D HangImageSize = { 100.f, 100.f };

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	TObjectPtr<UBackpackComponent> BackpackComp;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Inventory;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Hotbar;
};
