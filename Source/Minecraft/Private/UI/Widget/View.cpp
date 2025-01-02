// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/View.h"

void UView::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

void UView::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	SetInputModeUIOnly();
}

void UView::NativeDestruct()
{
	Super::NativeDestruct();

	SetInputModeGameOnly();
}

FReply UView::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::E || Key == EKeys::Escape)
	{
		RemoveFromParent();
	}

	return FReply::Handled();
}

void UView::SetInputModeUIOnly()
{
	APlayerController* PlayerController = GetOwningPlayer();

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetIgnoreLookInput(true);
}

void UView::SetInputModeGameOnly()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetIgnoreLookInput(false);
	}
}
