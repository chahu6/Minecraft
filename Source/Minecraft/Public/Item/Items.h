// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	~UItems();

	//UFUNCTION(BlueprintCallable) // À¶Í¼¿Éµ÷ÓÃ
	FORCEINLINE static UItems* Get()
	{
		if (Singleton == nullptr)
		{
			Initializer();
		}
		return Singleton;
	}

	//UPROPERTY()
	//TMap<int32, TObjectPtr<UItem>> ItemsMap;

private:
	static void Initializer();
	static void InitializeItems();

private:
	static UItems* Singleton;
};
