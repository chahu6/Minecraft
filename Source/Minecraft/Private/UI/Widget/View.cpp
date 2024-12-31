// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/View.h"

void UView::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
