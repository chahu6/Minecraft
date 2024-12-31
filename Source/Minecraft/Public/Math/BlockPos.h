// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockPos.generated.h"

struct FChunkPos;

/**
 * [0, 16]
 */
USTRUCT(BlueprintType)
struct MINECRAFT_API FBlockPos final
{
	GENERATED_BODY()

public:
	FBlockPos();
	FBlockPos(int32 InX, int32 InY);
	FBlockPos(int32 InX, int32 InY, int32 InZ);
	FBlockPos(const FVector& InPos);

	UPROPERTY()
	int32 X;
	
	UPROPERTY()
	int32 Y;

	UPROPERTY()
	int32 Z;

	FChunkPos ToChunkPos() const;

	int32 Index() const;

	FBlockPos Down() const;

	FBlockPos Up() const;
	FBlockPos Up(int32 InZ) const;

	bool operator!=(const FBlockPos& Other) const
	{
		return (X != Other.X) || (Y != Other.Y) || (Z != Other.Z);
	}

	bool operator==(const FBlockPos& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}

	FBlockPos operator+=(const FBlockPos& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;

		return *this;
	}

	FBlockPos operator+(const FBlockPos& Other) const
	{
		return FBlockPos(*this) += Other;
	}

	FBlockPos operator*=(int32 Scale)
	{
		X *= Scale;
		Y *= Scale;
		Z *= Scale;

		return *this;
	}

	FBlockPos operator*(int32 Scale) const
	{
		return FBlockPos(*this) *= Scale;
	}

	FORCEINLINE FVector ToVector() const;

	FORCEINLINE FString ToString() const { return FString::Printf(TEXT("X=%d Y=%d Z=%d"), X, Y, Z); }
};