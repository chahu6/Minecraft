// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items.generated.h"

class UItem;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UItems : public UObject
{
	GENERATED_BODY()
public:

public:
	~UItems();

	FORCEINLINE static UItems& Get()
	{
		if (Instance == nullptr)
		{
			Initialization();
		}
		return *Instance;
	}

	static void Initialization();

private:
	static UItems* Instance;
};
