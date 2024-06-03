// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/WindowsCriticalSection.h"
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
	UFUNCTION(BlueprintCallable) // À¶Í¼¿Éµ÷ÓÃ
	static UItems* Get();

	UPROPERTY()
	TMap<int32, TObjectPtr<UItem>> ItemsMap;
};
