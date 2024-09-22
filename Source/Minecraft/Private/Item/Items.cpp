// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Items.h"

UItems* UItems::Instance;

UItems::~UItems()
{
	Instance = nullptr;
}

void UItems::Initialization()
{
	Instance = NewObject<UItems>(GetTransientPackage(), NAME_None);
	Instance->AddToRoot();
}
