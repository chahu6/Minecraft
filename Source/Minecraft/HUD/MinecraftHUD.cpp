// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecraftHUD.h"
#include "Blueprint/UserWidget.h"

void AMinecraftHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AMinecraftHUD::DrawHUD()
{
	Super::DrawHUD();

}

void AMinecraftHUD::AddDebugInfo()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && DebugInfoClass)
	{
		DebugInfoUI = CreateWidget<UUserWidget>(PlayerController, DebugInfoClass);
		DebugInfoUI->AddToViewport();
	}
}

void AMinecraftHUD::RemoveDebugInfo()
{
	if (DebugInfoUI)
	{
		DebugInfoUI->RemoveFromParent();
	}
}
