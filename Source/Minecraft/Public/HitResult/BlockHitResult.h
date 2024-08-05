#pragma once

#include "CoreMinimal.h"
#include "Core/BlockPos.h"
#include "World/Block/Block.h"
#include "MCHitResult.h"

struct FBlockHitResult : public FMCHitResult
{
	FBlockData BlockData;

	bool bIsHit;

	FVector Direction;

	FBlockPos BlockPos;

	FBlockHitResult()
	{
		Init();
	}

	FORCEINLINE void Init()
	{
		FMemory::Memzero(this, sizeof(FBlockHitResult));
	}

	FORCEINLINE bool operator==(const FBlockHitResult& HitResult)
	{
		return this->BlockPos == HitResult.BlockPos;
	}

	FORCEINLINE int32 GetBlockID() const
	{
		return static_cast<int32>(BlockData.ID);
	}

	virtual EHitResultTpye GetType() override { return bIsHit ? EHitResultTpye::MISS : EHitResultTpye::BLOCK; }
};