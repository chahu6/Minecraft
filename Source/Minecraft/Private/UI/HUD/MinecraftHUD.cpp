#include "UI/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/StorageUI/Backpack.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/MainUI.h"

void AMinecraftHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMinecraftHUD::DrawHUD()
{
	Super::DrawHUD();
	
	DrawCrosshairs();
}

void AMinecraftHUD::DrawCrosshairs()
{
	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
		if (Crosshairs)
		{
			const float TextureWidget = Crosshairs->GetSizeX();
			const float TextureHeight = Crosshairs->GetSizeY();

			const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidget / 2.0f), ViewportCenter.Y - (TextureHeight / 2.0f));

			DrawTexture(
				Crosshairs,
				TextureDrawPoint.X,
				TextureDrawPoint.Y,
				TextureWidget,
				TextureHeight,
				0.0f,
				0.0f,
				1.0f,
				1.0f
			);
		}
	}
}

UOverlayWidgetController* AMinecraftHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
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

void AMinecraftHUD::AddBackpack()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && BackpackClass)
	{
		BackpackUI = CreateWidget<UBackpack>(PlayerController, BackpackClass);
		BackpackUI->AddToViewport();
	}
}

void AMinecraftHUD::RemoveBackpack()
{
	if (BackpackUI)
	{
		BackpackUI->RemoveFromParent();
	}
}

void AMinecraftHUD::InitMainUI(APlayerController* PC, APlayerState* PS, APawn* Pawn)
{
	checkf(MainUIClass, TEXT("MainUIClass Class uninitialized"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized"));

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		MainUI = CreateWidget<UMainUI>(PlayerController, MainUIClass);

		const FWidgetControllerParams WCParams(PC, PS, Pawn);
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParams);
		check(WidgetController);

		MainUI->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValue();

		MainUI->AddToViewport();
	}
}
