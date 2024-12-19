// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/BlockPos.h"
#include "Math/ChunkPos.h"

FBlockPos::FBlockPos()
{
	X = Y = Z = 0;
}

FBlockPos::FBlockPos(int32 InX, int32 InY)
	:X(InX), Y(InY), Z(0)
{
}

FBlockPos::FBlockPos(const FVector& InPos)
	:X(FMath::TruncToInt32(InPos.X)),
	Y(FMath::TruncToInt32(InPos.Y)),
	Z(FMath::TruncToInt32(InPos.Z))
{
}

FChunkPos FBlockPos::ToChunkPos() const
{
	return FChunkPos(*this);
}
