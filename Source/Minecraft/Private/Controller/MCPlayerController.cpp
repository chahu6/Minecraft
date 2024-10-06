#include "Controller/MCPlayerController.h"
#include "UI/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "Controller/CameraManager/MinecraftPlayerCameraManager.h"
#include "SimplexNoiseLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widget/ProgressBarWidget.h"
#include "Components/ProgressBar.h"

AMCPlayerController::AMCPlayerController()
{
	PlayerCameraManagerClass = AMinecraftPlayerCameraManager::StaticClass();
}

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MinecraftHUD = GetHUD<AMinecraftHUD>();
}

void AMCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AMCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowDebugInfo", IE_Pressed, this, &AMCPlayerController::ShowDebugInfo);
}

void AMCPlayerController::InitMainUI_Implementation()
{
	MinecraftHUD = MinecraftHUD == nullptr ? GetHUD<AMinecraftHUD>() : MinecraftHUD;
	if (MinecraftHUD)
	{
		MinecraftHUD->InitMainUI(this, GetPawn()->GetPlayerState(), GetPawn());
	}
}

void AMCPlayerController::AddPrograssWidget()
{
	if (ProgressBarWidgetClass)
	{
		ProgressBarWidget = CreateWidget<UProgressBarWidget>(this, ProgressBarWidgetClass);
		ProgressBarWidget->AddToViewport();
	}
}

void AMCPlayerController::RemovePrograssWidget()
{
	if (ProgressBarWidget)
	{
		ProgressBarWidget->RemoveFromParent();
	}
}

void AMCPlayerController::SetPrograssPercent(float Percent)
{
	if (ProgressBarWidget && ProgressBarWidget->ProgressBar)
	{
		ProgressBarWidget->ProgressBar->SetPercent(Percent);
	}
}

void AMCPlayerController::DisplayGUI(const TSubclassOf<UUserWidget>& UserWidgetClass)
{
	if (UserWidgetClass)
	{
		//OpenContainer = CreateWidget
	}
}

void AMCPlayerController::ShowDebugInfo()
{
	if (bIsDebug)
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->RemoveDebugInfo();
			bIsDebug = false;
		}
	}
	else
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->AddDebugInfo();
			bIsDebug = true;
		}
	}
}