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
	FVector VoxelLocalPosition; // Chunk下的相对坐标
	FVector VoxelWorldPosition; // 体素坐标下的世界坐标，没有乘以BlockSize
	FVector ChunkVoexlWorldPosition;
};

class AMCCharacter;
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
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

	void AddBlock();
	void RemoveBlock();

	UFUNCTION(BlueprintCallable)
	UTexture2D* CreateTextureFromArray();
private:
	void ShowDebugInfo();

	void RayCast();

	uint8 GetBlockID(const FVector& VoxelWorldPosition, FBlockData& OutBlockData);
	
	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

	void Rebuild_Adjacent_Chunks();

private:
	UPROPERTY()
	class AMinecraftHUD* MinecraftHUD;

	bool bIsDebug = false;

	FBlockData BlockData;

	FBlockData Temp;

	UPROPERTY(EditAnywhere, Category = "AAA")
	float Factor = 0.05f;

	UPROPERTY(EditAnywhere, Category = "AAA")
	float FudgeFactor = 1.2f;

	UPROPERTY(EditAnywhere, Category = "AAA")
	float Exponent = 1.0f;

	UPROPERTY(EditAnywhere, Category = "AAA")
	int32 Seed;
};
