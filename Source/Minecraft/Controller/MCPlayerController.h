// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

USTRUCT()
struct FBlockData
{
	GENERATED_USTRUCT_BODY()

	uint8 BlockID;
	FVector Normal;
	int32 BlockIndex;
	int32 ChunkIndex;
};

class AMCPlayer;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;


	void AddBlock();
	void RemoveBlock();
private:
	void ShowDebugInfo();

	void RayCast();

	bool GetBlockData(const FVector& VoxelWorldPosition);
	
private:
	UPROPERTY()
	class AMinecraftHUD* MinecraftHUD;

	UPROPERTY()
	AMCPlayer* MCPlayer;

	bool bIsOpened = false;

	FBlockData BlockData;
};
