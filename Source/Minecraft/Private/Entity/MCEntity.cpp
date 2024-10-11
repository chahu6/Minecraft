// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/MCEntity.h"

AMCEntity::AMCEntity()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMCEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

