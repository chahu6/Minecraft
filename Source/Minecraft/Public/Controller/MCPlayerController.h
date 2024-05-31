#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCPlayerController();
	
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

public:
	void OpenBackpack();

	UFUNCTION(BlueprintCallable)
	UTexture2D* CreateTextureFromArray();

private:
	void ShowDebugInfo();

private:
	UPROPERTY()
	class AMinecraftHUD* MinecraftHUD;

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
