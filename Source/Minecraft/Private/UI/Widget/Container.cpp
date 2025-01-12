// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Container.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/Widget/DroppableInventoryCellWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UContainer::SetActor(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
}

void UContainer::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	SetInputModeUIOnly();
}

void UContainer::NativeDestruct()
{
	Super::NativeDestruct();

	SetInputModeGameOnly();
}

FReply UContainer::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::E || Key == EKeys::Escape)
	{
		RemoveFromParent();
	}

	return FReply::Handled();
}

void UContainer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(HangItem))
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(HangItem->Slot))
		{
			FVector2D Location;
			UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), Location.X, Location.Y);

			CanvasPanelSlot->SetPosition(Location - (HangImageSize / 2.f));
		}
	}
}

void UContainer::SlotClick_Implementation(USlot* ClickedSlot, EMouseEvent MouseEvent)
{
	if (UInventoryWidgetController* InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController))
	{
		InventoryWidgetController->SlotClick(ClickedSlot, MouseEvent);
	}
}
