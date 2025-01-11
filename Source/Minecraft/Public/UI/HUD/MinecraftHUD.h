#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MinecraftHUD.generated.h"

class UOverlayWidgetController;
struct FWidgetControllerParams;
class UMainUI;
class UBagWidgetController;
class UContainer;
class UWorkbenchWidgetController;
class UMinecraftWidgetController;

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMinecraftHUD : public AHUD
{
	GENERATED_BODY()

	friend class AMCPlayerController;

public:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UBagWidgetController* GetBagWidgetController(const FWidgetControllerParams& WCParams);
	UWorkbenchWidgetController* GetWorkbenchWidgetController(const FWidgetControllerParams& WCParams);

	void DisplayBag(AActor* OwnerActor);

	void DisplayGUI(TSubclassOf<UContainer> WidgetClass, UMinecraftWidgetController* WidgetController, AActor* OwnerActor);

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
	TSubclassOf<UBagWidgetController> BagWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UBagWidgetController> BagWidgetController;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UWorkbenchWidgetController> WorkbenchWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UWorkbenchWidgetController> WorkbenchWidgetController;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMainUI> MainUIClass;

	UPROPERTY()
	TObjectPtr<UMainUI> MainUI;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UContainer> BagWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UContainer> OpenWidget;

	// Ê®×Ö×¼ÐÇ
	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	TObjectPtr<UTexture2D> Crosshairs;
};
