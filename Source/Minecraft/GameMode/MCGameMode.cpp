// Fill out your copyright notice in the Description page of Project Settings.


#include "MCGameMode.h"
#include "Minecraft/WorldManager/WorldManager.h"

void AMCGameMode::StartPlay()
{
	Super::StartPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		if (WorldManagerClass)
		{
			World->SpawnActor<AWorldManager>(WorldManagerClass);
		}
	}
}


void AMCGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
