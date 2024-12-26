// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items.generated.h"

class UItem;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UItems : public UObject
{
	GENERATED_BODY()
public:
	static const UItem* Air;

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
	static UItem* GetRegisteredItem(const FGameplayTag& InItemID);

private:
	static UItems* Instance;
};
