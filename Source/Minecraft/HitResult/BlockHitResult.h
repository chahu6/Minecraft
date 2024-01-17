#pragma once

#include "CoreMinimal.h"
#include "Minecraft/Core/BlockPos.h"
#include "MCHitResult.h"

struct FBlockHitResult : public FMCHitResult
{
	uint8 BlockID;

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

	virtual EHitResultTpye GetType() override { return bIsHit ? EHitResultTpye::MISS : EHitResultTpye::BLOCK; }
};