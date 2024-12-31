// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class ENBTTagType : uint8
{
	TERMINAL,
	BYTE,
	SHORT,
	LONG,
	FLOAT,
	DOUBLE,
	INT32,
	BYTE_ARRAY,
	INT16_ARRAY,
	INT32_ARRAY,
	COMPOUND,
	CHUNK_BLOCK_TENSOR,
	CHUNK_BIOME
};

/**
 * 
 */
class MINECRAFT_API KNBTBase
{
public:
	KNBTBase(const FString& InTagName) :TagName(InTagName) {}
	virtual ~KNBTBase() = default;

	virtual void Read(FArchive& Reader) = 0;
	virtual void Write(FArchive& Writer) = 0;
	virtual ENBTTagType GetId() const = 0;

	KNBTBase* NewTagFromTagId(uint8 Id, const FString& Name);

public:
	FString TagName;
};

template<typename T>
class KNBTTag : public KNBTBase
{
public:
	KNBTTag(const FString& InTagName, T InValue = T())
		:KNBTBase(InTagName), Value(InValue) {}

	virtual void Read(FArchive& Reader) override
	{
		Reader << Value;
	}
	virtual void Write(FArchive& Writer) override
	{
		Writer << Value;
	}

	virtual ENBTTagType GetId() const override
	{
		if constexpr(std::is_same<T, uint8>::value)
		{
			return ENBTTagType::BYTE;
		}
		else if constexpr (std::is_same<T, int16>::value)
		{
			return ENBTTagType::SHORT;
		}
		else if constexpr (std::is_same<T, int64>::value)
		{
			return ENBTTagType::LONG;
		}
		else if constexpr (std::is_same<T, float>::value)
		{
			return ENBTTagType::FLOAT;
		}
		else if constexpr (std::is_same<T, double>::value)
		{
			return ENBTTagType::DOUBLE;
		}
		else if constexpr(std::is_same<T, int32>::value)
		{
			return ENBTTagType::INT32;
		}
		return ENBTTagType::TERMINAL;
	}

public:
	T Value;
};
