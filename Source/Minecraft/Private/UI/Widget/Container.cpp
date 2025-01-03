// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Container.h"

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
