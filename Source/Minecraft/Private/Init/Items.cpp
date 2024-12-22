// Fill out your copyright notice in the Description page of Project Settings.


#include "Init/Items.h"
#include "Item/Item.h"
#include "MinecraftGameplayTags.h"

UItems* UItems::Instance;

const UItem* UItems::Air;

UItems::~UItems()
{
	Instance = nullptr;
}

void UItems::Initialization()
{
	Instance = NewObject<UItems>(GetTransientPackage(), NAME_None);
	Instance->AddToRoot();

	const FMinecraftGameplayTags& GameplayTags = FMinecraftGameplayTags::Get();

	Air = GetRegisteredItem(GameplayTags.Air.GetTagName());
}

UItem* UItems::GetRegisteredItem(const FName& Name)
{
	if (UItem::REGISTER.Contains(Name))
	{
		return UItem::REGISTER[Name];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Invalid Item Requested: %s"), *Name.ToString());
	}
	return nullptr;
}
