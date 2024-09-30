#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MinecraftHUD.generated.h"

class UOverlayWidgetController;
struct FWidgetControllerParams;
class UMinecraftUserWidget;
class UMainUI;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMinecraftHUD : public AHUD
{
	GENERATED_BODY()

private:
	friend class AMCPlayerController;

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

public:
	void AddDebugInfo();

	void RemoveDebugInfo();

	void DrawCrosshairs();

	void InitMainUI(APlayerController* PC, APlayerState* PS, APawn* Pawn);

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DebugInfoClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> DebugInfoUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMainUI> MainUIClass;

	UPROPERTY()
	TObjectPtr<UMainUI> MainUI;

	// Ê®×Ö×¼ÐÇ
	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	TObjectPtr<UTexture2D> Crosshairs;
};
