#include "MCPlayerController.h"
#include "Minecraft/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "Minecraft/Character/MCPlayer.h"
#include "Camera/CameraComponent.h"
#include "Minecraft/World/WorldSettings.h"

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MinecraftHUD = Cast<AMinecraftHUD>(GetHUD());
}

void AMCPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MCPlayer = Cast<AMCPlayer>(GetPawn());
}

void AMCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowDebugInfo", IE_Pressed, this, &AMCPlayerController::ShowDebugInfo);
	InputComponent->BindAction("RayCast", IE_Pressed, this, &AMCPlayerController::RayCast);
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

void AMCPlayerController::RayCast()
{

}
