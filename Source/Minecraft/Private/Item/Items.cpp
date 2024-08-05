// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Items.h"
#include "Utils/MinecraftAssetLibrary.h"
//#include "World/Block/Blocks.h"

UItems* UItems::Singleton = nullptr;

UItems::~UItems()
{
    Singleton = nullptr;
}

void UItems::Initializer()
{
    Singleton = NewObject<UItems>(GetTransientPackage(), NAME_None);
    Singleton->AddToRoot();
}

void UItems::InitializeItems()
{
    /*UItems* Items = UItems::Get();
    UAirItem* AirItem = NewObject<UAirItem>();
    AirItem->ItemInfo = UMinecraftAssetLibrary::GetItemInfo(0);
    Items->ItemsMap.Add(0, AirItem);

    UBlockItem* Stone = NewObject<UBlockItem>();
    Stone->ItemInfo = UMinecraftAssetLibrary::GetItemInfo(1);
    Items->ItemsMap.Add(1, Stone);

    UBlockItem* Dirt = NewObject<UBlockItem>();
    Dirt->ItemInfo = UMinecraftAssetLibrary::GetItemInfo(2);
    Items->ItemsMap.Add(2, Dirt);

    UBlockItem* Grass = NewObject<UBlockItem>();
    Grass->ItemInfo = UMinecraftAssetLibrary::GetItemInfo(3);
    Items->ItemsMap.Add(3, Grass);*/
}
