// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Block/Data/BlockData.h"
#include "World/Data/FaceType.h"

class UBlock;

class VoxelNeighbours
{

};

/**
 * 
 */
struct MINECRAFT_API FBlockState
{
public:
	FBlockState();
	FBlockState(const UBlock* InBlock);
	FBlockState(const UBlock* InBlock, const FIntVector& InPosition);

public:
	int32 BlockID;

	EFaceType FaceType;

	FIntVector Position;

private:
	UBlock* Block = nullptr;

public:
	UBlock* GetBlock();

	void SetBlock(const UBlock* InBlock);

	FORCEINLINE bool IsAir() const;
};