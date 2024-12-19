// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkPos.generated.h"

struct FBlockPos;

/**
 * [0, 1]
 */
USTRUCT(BlueprintType)
struct MINECRAFT_API FChunkPos
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 X;

	UPROPERTY()
	int32 Y;

public:
	FChunkPos();
	FChunkPos(int32 InX, int32 InY);
	explicit FChunkPos(const FBlockPos& InBlockPos);

	FBlockPos ToBlockPos() const;

	bool operator!=(const FChunkPos& Other) const 
	{ 
		return (X != Other.X) || (Y != Other.Y);
	}

	bool operator==(const FChunkPos& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	FChunkPos operator+=(const FChunkPos& Other)
	{
		X += Other.X;
		Y += Other.Y;

		return *this;
	}

	FChunkPos operator+(const FChunkPos& Other) const
	{
		return FChunkPos(*this) += Other;
	}

	FChunkPos operator*=(int32 Scale)
	{
		X *= Scale;
		Y *= Scale;

		return *this;
	}

	FChunkPos operator*(int32 Scale) const
	{
		return FChunkPos(*this) *= Scale;
	}

	friend inline uint32 GetTypeHash(const FChunkPos& Key)
	{
		return HashCombine(::GetTypeHash(Key.X), ::GetTypeHash(Key.Y));
	}

	FORCEINLINE FString ToString() { return FString::Printf(TEXT("X=%d Y=%d"), X, Y); }
};
