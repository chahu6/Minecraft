// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileEntity.generated.h"

class UBlock;
class AWorldManager;
class AEntityPlayer;

/**
 * 
 */
UCLASS()
class MINECRAFT_API ATileEntity : public AActor
{
	GENERATED_BODY()
public:
	ATileEntity();

	/** 当玩家右键单击该块时调用 */
	virtual void OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnBlockActivated"))
	void ReceiveOnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player);

protected:
	UPROPERTY()
	TObjectPtr<UBlock> Block;

public:
	FORCEINLINE void SetBlock(UBlock* InBlock) { Block = InBlock; }
	FORCEINLINE UBlock* GetBlock() const { return Block; }
};
