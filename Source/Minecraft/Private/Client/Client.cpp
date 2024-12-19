// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/Client.h"

AClient::AClient()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AClient::BeginPlay()
{
	Super::BeginPlay();
	
}

void AClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

