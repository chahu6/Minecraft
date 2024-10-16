// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Data/BlockState.h"
#include "World/Block/Block.h"
#include "Init/Blocks.h"

FBlockState::FBlockState()
	:BlockID(0),
	FaceType(EFaceType::Forward),
	Position{ 0, 0, 0 }
{
	SetBlock(UBlocks::Air);
}

FBlockState::FBlockState(const UBlock * InBlock)
	:BlockID(InBlock->BlockID)
{
	SetBlock(InBlock);
}

FBlockState::FBlockState(const UBlock* InBlock, const FIntVector& InPosition)
	:BlockID(InBlock->BlockID),
	Position(InPosition)
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
	return BlockID == 0;
}

float FBlockState::GetPlayerRelativeBlockHardness()
{
	return Block->GetPlayerRelativeBlockHardness();
}
