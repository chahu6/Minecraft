// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MCGameMode.h"
#include "World/WorldManager.h"
#include "Controller/MCPlayerController.h"
#include "Player/MinecraftPlayer.h"

/**
 *	执行的先后顺序
 */

void AMCGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AMCGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMCGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMCGameMode::OnPostLogin(AController* NewPlayer)
{
	if (AMCPlayerController* PlayerController = Cast<AMCPlayerController>(NewPlayer))
	{
		//PlayerController->SetPrograssPercent(0.f);
		//PlayerController->AddPrograssWidget();
		//EnterWorld(PlayerController);
	}
}

void AMCGameMode::StartPlay()
{
	Super::StartPlay();
}

void AMCGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMCGameMode::EnterWorld(AMCPlayerController* NewPlayer)
{
	check(MinecraftPlayerClass);
	check(WorldManagerClass);

	AWorldManager* WorldManager = GetWorld()->SpawnActorDeferred<AWorldManager>(WorldManagerClass, FTransform());
	WorldManager->ProgressDelegate.BindLambda([=](float Percent) {
		NewPlayer->SetPrograssPercent(Percent);
		if (Percent == 1.f)
		{
			NewPlayer->RemovePrograssWidget();
			FVector2D DefaultCharacterPosition = WorldManager->GetDefaultCharacterPosition();

			AMinecraftPlayer* PlayerCharacter = GetWorld()->SpawnActorDeferred<AMinecraftPlayer>(MinecraftPlayerClass, FTransform());
			if (PlayerCharacter == nullptr) return;

			NewPlayer->SetPawn(PlayerCharacter);
			NewPlayer->OnPossess(PlayerCharacter);
			PlayerCharacter->FinishSpawning(FTransform(FVector(DefaultCharacterPosition, 15000.f)));

			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult, FVector(DefaultCharacterPosition, 100000.0f), FVector(DefaultCharacterPosition, -1000.f), ECollisionChannel::ECC_Visibility);
			if (HitResult.bBlockingHit)
			{
				PlayerCharacter->SetActorLocation(HitResult.ImpactPoint + FVector(0.f, 0.f, 2 * PlayerCharacter->GetDefaultHalfHeight()));
			}
		}
	});
	WorldManager->FinishSpawning(FTransform());
}