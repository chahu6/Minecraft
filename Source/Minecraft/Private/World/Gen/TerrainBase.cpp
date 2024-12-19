// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Gen/TerrainBase.h"
#include "SimplexNoiseLibrary.h"

void UTerrainBase::SetSeed(int32 NewSeed)
{
	Seed = NewSeed;
	USimplexNoiseLibrary::SetNoiseSeed(Seed);
}
