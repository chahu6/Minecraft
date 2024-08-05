#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "MCPlayerController.generated.h"

class AMinecraftHUD;
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

	virtual void InitMainUI_Implementation() override;

	virtual void OpenBackpack_Implementation() override;

public:
	UFUNCTION(BlueprintCallable)
	UTexture2D* CreateTextureFromArray();

private:
	void ShowDebugInfo();

private:
	UPROPERTY()
	TObjectPtr<AMinecraftHUD> MinecraftHUD;

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
