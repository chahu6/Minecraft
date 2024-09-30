#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "MCPlayerController.generated.h"

class AMinecraftHUD;
class UProgressBarWidget;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

public:
	AMCPlayerController();
	
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

	/** Player Controller Interface */
	virtual void InitMainUI_Implementation() override;
	/** end Player Controller Interface */

	void AddPrograssWidget();
	void RemovePrograssWidget();
	void SetPrograssPercent(float Percent);

public:
#if 0
	UFUNCTION(BlueprintCallable)
	UTexture2D* CreateTextureFromArray();
#endif
private:
	void ShowDebugInfo();

protected:
	UPROPERTY()
	TObjectPtr<AMinecraftHUD> MinecraftHUD;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UProgressBarWidget> ProgressBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UProgressBarWidget> ProgressBarWidget;

	UPROPERTY(EditAnywhere, Category = "AAA")
	float Factor = 0.05f;

	UPROPERTY(EditAnywhere, Category = "AAA")
	float FudgeFactor = 1.2f;

	UPROPERTY(EditAnywhere, Category = "AAA")
	float Exponent = 1.0f;

	UPROPERTY(EditAnywhere, Category = "AAA")
	int32 Seed;

	bool bIsDebug = false;
};
