// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MCGameMode.h"
#include "World/WorldManager.h"
#include "Controller/MCPlayerController.h"
#include "Player/EntityPlayer.h"
#include "Blueprint/UserWidget.h"

#include "UI/Widget/ProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "GameInstance/MinecraftGameInstance.h"

/**
 *	执行的先后顺序
 */

AMCGameMode::AMCGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

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
}

void AMCGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	EnterWorld(NewPlayer);
}

void AMCGameMode::StartPlay()
{
	Super::StartPlay();
}

void AMCGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMCGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCGameMode::EnterWorld(APlayerController* NewPlayer)
{
	check(WorldManagerClass);

	//AWorldManager* WorldManager = GetWorld()->SpawnActorDeferred<AWorldManager>(WorldManagerClass, FTransform());
	//WorldManager->ProgressDelegate.BindLambda([&](float Percent) {
	//	//SetPrograssPercent(Percent);
	//	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red, FString::Printf(TEXT("Percent: %f"), Percent));
	//	if (Percent == 1.f)
	//	{
	//		if (ProgressBarWidget)
	//		{
	//			ProgressBarWidget->RemoveFromParent();
	//		}
	//		WorldManager->ProgressDelegate.Unbind();
	//	}
	//});

	//NewPlayer->GetPawn()->SetActorLocation(FVector(0, 0, 35000));

	//WorldManager->FinishSpawning(FTransform());
}

void AMCGameMode::SetPrograssPercent(float Percent)
{
	if (ProgressBarWidget && ProgressBarWidget->ProgressBar)
	{
		ProgressBarWidget->ProgressBar->SetPercent(Percent);
	}
}
