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

	Air = GetRegisteredItem(GameplayTags.Air);
}

UItem* UItems::GetRegisteredItem(const FGameplayTag& InItemID)
{
	if (UItem::REGISTER.Contains(InItemID))
	{
		return UItem::REGISTER[InItemID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Item Requested: %s"), *InItemID.ToString());
	}
	return nullptr;
}
