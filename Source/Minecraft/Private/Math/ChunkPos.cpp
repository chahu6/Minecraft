// Fill out your copyright notice in the Description page of Project Settings.


#include "Math/ChunkPos.h"
#include "Math/BlockPos.h"
#include "World/WorldGenerator.h"

FChunkPos::FChunkPos()
{
    X = Y = 0;
}

FChunkPos::FChunkPos(int32 InX, int32 InY)
    :X(InX), Y(InY)
{
}

FChunkPos::FChunkPos(const FBlockPos& InBlockPos)
{
    X = FMath::FloorToInt32(static_cast<float>(InBlockPos.X) / 16);
    Y = FMath::FloorToInt32(static_cast<float>(InBlockPos.Y) / 16);
}

FBlockPos FChunkPos::ToBlockPos() const
{
    return FBlockPos(X * WorldGenerator::CHUNK_SIZE, Y * WorldGenerator::CHUNK_SIZE);
}

FVector FChunkPos::ToVector() const
{
    return FVector(X, Y, 0.f) * WorldGenerator::ChunkSize;
}
