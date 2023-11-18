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
	FVector VoxelLocalPosition; // Chunk�µ��������
	FVector VoxelWorldPosition; // ���������µ��������꣬û�г���BlockSize
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

	uint8 GetBlockID(const FVector& VoxelWorldPosition, FBlockData& OutBlockData);
	
	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

	void Rebuild_Adjacent_Chunks();

private:
	UPROPERTY()
	class AMinecraftHUD* MinecraftHUD;

	UPROPERTY()
	AMCPlayer* MCPlayer;

	bool bIsDebug = false;

	FBlockData BlockData;
};
