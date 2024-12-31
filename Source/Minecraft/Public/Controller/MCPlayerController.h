#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

class AMinecraftHUD;
class UProgressBarWidget;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCPlayerController();
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;

	void AddPrograssWidget();
	void RemovePrograssWidget();
	void SetPrograssPercent(float Percent);

	void DisplayGUI(const TSubclassOf<UUserWidget>& UserWidgetClass);

private:
	void ShowDebugInfo();

protected:
	UPROPERTY()
	TObjectPtr<AMinecraftHUD> MinecraftHUD;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UProgressBarWidget> ProgressBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UProgressBarWidget> ProgressBarWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> OpenContainer;

	//UPROPERTY(EditAnywhere, Category = "AAA")
	//float Factor = 0.05f;

	//UPROPERTY(EditAnywhere, Category = "AAA")
	//float FudgeFactor = 1.2f;

	//UPROPERTY(EditAnywhere, Category = "AAA")
	//float Exponent = 1.0f;

	//UPROPERTY(EditAnywhere, Category = "AAA")
	//int32 Seed;

	bool bIsDebug = false;
};
