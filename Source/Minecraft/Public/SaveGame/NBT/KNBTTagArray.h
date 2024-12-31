// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/NBT/KNBTBase.h"

/**
 * 
 */
template<typename T>
class MINECRAFT_API KNBTTagArray : public KNBTBase
{
public:
	KNBTTagArray(const FString& InTagName, const TArray<T>& InValue = TArray<T>())
		:KNBTBase(InTagName), Value(InValue)
	{}

	virtual void Read(FArchive& Reader) override
	{
		int32 Num = 0;
		Reader << Num;
		Value.Init(0, Num);
		for (int32 i = 0; i < Num; ++i)
		{
			Reader << Value[i];
		}
	}

	virtual void Write(FArchive& Writer) override
	{
		int32 Num = Value.Num();
		Writer << Num;
		for (int32 i = 0; i < Num; ++i)
		{
			Writer << Value[i];
		}
	}

	virtual ENBTTagType GetId() const override;

public:
	TArray<T> Value;
};
