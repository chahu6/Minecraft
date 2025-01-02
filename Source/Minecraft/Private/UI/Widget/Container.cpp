// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Container.h"
#include "Interfaces/InventoryInterface.h"

void UContainer::SetInventoryInterface(TScriptInterface<UInventoryInterface> InInventoryInterface)
{
	InventoryInterface = InInventoryInterface;
}
