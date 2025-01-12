#include "Controller/MCPlayerController.h"
#include "UI/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "Controller/CameraManager/MinecraftPlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widget/ProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AMCPlayerController::AMCPlayerController()
{
	PlayerCameraManagerClass = AMinecraftPlayerCameraManager::StaticClass();
}

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ensure(PlayerControllerMappingContext);

	MinecraftHUD = GetHUD<AMinecraftHUD>();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerControllerMappingContext, 0);
	}
}

void AMCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AMCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Êó±ê¹öÂÖ
		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &AMCPlayerController::SwitchingWheel);
	}

	InputComponent->BindAction("ShowDebugInfo", IE_Pressed, this, &AMCPlayerController::ShowDebugInfo);
}

bool AMCPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	return Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);
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

void AMCPlayerController::SwitchingWheel(const FInputActionValue& Value)
{
	const int32 WheelValue = -Value.Get<float>();

	OnSwitchWheelDelegate.Broadcast(WheelValue);
}
