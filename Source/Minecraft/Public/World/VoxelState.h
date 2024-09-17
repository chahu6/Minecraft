// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Block/Block.h"

/**
 * 
 */
class MINECRAFT_API VoxelState
{
public:
	VoxelState();
	~VoxelState();

	FBlockData BlockData;
};
