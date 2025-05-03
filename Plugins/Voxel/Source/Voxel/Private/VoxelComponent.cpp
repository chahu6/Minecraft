// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelComponent.h"

UVoxelComponent::UVoxelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVoxelComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVoxelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

