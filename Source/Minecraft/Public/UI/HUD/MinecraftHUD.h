#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MinecraftHUD.generated.h"

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

public:
	void AddDebugInfo();

	void RemoveDebugInfo();

	void AddBackpack();

	void RemoveBackpack();

	void DrawCrosshairs();

	void AddMainUI();
private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DebugInfoClass;

	UPROPERTY()
	class UUserWidget* DebugInfoUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> BackpackClass;

	UPROPERTY()
	class UBackpack* BackpackUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainClass;

	UPROPERTY()
	class UUserWidget* MainUI;

	// ʮ��׼��
	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	UTexture2D* Crosshairs;
};