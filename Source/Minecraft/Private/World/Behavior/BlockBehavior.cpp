// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Behavior/BlockBehavior.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"

void UBlockBehavior::OnInteract()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UBlockBehavior::OnInteract()")));
}

void UBlockBehavior::OnBeforePlace()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UBlockBehavior::OnBeforePlace()")));
}

void UBlockBehavior::OnAfterPlace(AWorldManager* WorldManager, const FVector& Location, USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(WorldManager, Sound, Location);
}

void UBlockBehavior::OnDestroy(AWorldManager* WorldManager, const FVector& Location, USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(WorldManager, Sound, Location);
}