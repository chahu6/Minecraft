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
	UFUNCTION(BlueprintCallable) // ��ͼ�ɵ���
	static UItems* Get();

	static void InitializeItems();

	UPROPERTY()
	TMap<int32, TObjectPtr<UItem>> ItemsMap;
};
