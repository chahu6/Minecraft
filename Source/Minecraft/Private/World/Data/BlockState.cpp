// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Data/BlockState.h"
#include "World/Block/Block.h"
#include "Init/Blocks.h"
#include "MinecraftGameplayTags.h"

FBlockState::FBlockState()
	:FaceType(EFaceType::Forward),
	Position{ 0, 0, 0 }
{
	SetBlock(UBlocks::Air);
}

FBlockState::FBlockState(const UBlock * InBlock)
{
	SetBlock(InBlock);
}

FBlockState::FBlockState(const UBlock* InBlock, const FIntVector& InPosition)
	:Position(InPosition)
{
	SetBlock(InBlock);
}

UBlock* FBlockState::GetBlock() const
{
	return Block;
}

void FBlockState::SetBlock(const UBlock* InBlock)
{
	Block = const_cast<UBlock*>(InBlock);
}

bool FBlockState::IsAir() const
{
	return Block->BlockID == AIR;
}

float FBlockState::GetPlayerRelativeBlockHardness()
{
	return Block->GetPlayerRelativeBlockHardness();
}
