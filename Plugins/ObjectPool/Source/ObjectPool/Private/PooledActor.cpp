// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActor.h"

APooledActor::APooledActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APooledActor::BeginPlay()
{
	Super::BeginPlay();
	
	//SetInUse(false);
}

void APooledActor::SetInUse(bool InUse)
{
	bInUse = InUse;
	SetActorEnableCollision(bInUse);
	SetActorHiddenInGame(!bInUse);
	SetActorTickEnabled(bInUse);

	GetWorldTimerManager().ClearTimer(TimeToLiveHandle);
	if (bInUse)
	{
		GetWorldTimerManager().SetTimer(TimeToLiveHandle, [this]() {
			SetInUse(false);
		}, TimeToLive, false);
	}

	ReceiveSetInUse(InUse);
}
