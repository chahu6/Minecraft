// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/BlockPos.h"
#include "Math/ChunkPos.h"
#include "World/WorldGenerator.h"

FBlockPos::FBlockPos()
{
	X = Y = Z = 0;
}

FBlockPos::FBlockPos(int32 InX, int32 InY)
	:X(InX), Y(InY), Z(0)
{
}

FBlockPos::FBlockPos(int32 InX, int32 InY, int32 InZ)
	:X(InX), Y(InY), Z(InZ)
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

int32 FBlockPos::Index() const
{
	return X + Y * WorldGenerator::CHUNK_SIZE + Z * WorldGenerator::CHUNK_AREA;
}

FBlockPos FBlockPos::Down() const
{
	return FBlockPos(X, Y, Z - 1);
}

FBlockPos FBlockPos::Up() const
{
	return Up(1);
}

FBlockPos FBlockPos::Up(int32 InZ) const
{
	return FBlockPos(X, Y, Z + InZ);
}

FVector FBlockPos::ToVector() const
{
	return FVector(X, Y, Z) * WorldGenerator::BlockSize;
}
