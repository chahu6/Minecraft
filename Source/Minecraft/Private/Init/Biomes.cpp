// Fill out your copyright notice in the Description page of Project Settings.


#include "Init/Biomes.h"
#include "World/Biome/Biome.h"

UBiomes* UBiomes::Instance;

UBiomes::~UBiomes()
{
	Instance = nullptr;
}

void UBiomes::Initialization()
{
	Instance = NewObject<UBiomes>(GetTransientPackage(), NAME_None);
	Instance->AddToRoot();
}

UBiome* UBiomes::GetRegisteredBiome(const FName& Name)
{

	return nullptr;
}
