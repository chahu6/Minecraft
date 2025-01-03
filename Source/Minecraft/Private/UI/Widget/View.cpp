// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/View.h"

void UView::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
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
