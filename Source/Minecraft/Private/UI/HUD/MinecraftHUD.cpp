#include "UI/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/StorageUI/Backpack.h"

void AMinecraftHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMinecraftHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrosshairs();
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

void AMinecraftHUD::AddMainUI()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && MainClass)
	{
		MainUI = CreateWidget<UUserWidget>(PlayerController, MainClass);
		MainUI->AddToViewport();
	}
}
