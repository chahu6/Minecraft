#include "MCPlayerController.h"
#include "Minecraft/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MinecraftHUD = Cast<AMinecraftHUD>(GetHUD());
}

void AMCPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowDebugInfo", IE_Pressed, this, &AMCPlayerController::ShowDebugInfo);
}

void AMCPlayerController::ShowDebugInfo()
{
	if (bIsOpened)
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->RemoveDebugInfo();
			bIsOpened = false;
		}
	}
	else
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->AddDebugInfo();
			bIsOpened = true;
		}
	}
}
