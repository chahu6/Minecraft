// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemStack.generated.h"

class UItem;
/**
 * 
 */
USTRUCT(BlueprintType)
struct MINECRAFT_API FItemStack final
{
	GENERATED_USTRUCT_BODY()

public:
	FItemStack();

	void Empty();

	bool IsEmpty() const;

	//bool IsFull() const;

	int32 GetCount() const;
	void SetCount(int32 Size);

	UItem* GetItem() const;
	void SetItemID(const FGameplayTag& InItemID);
	FGameplayTag GetItemID() const;

	bool IsStack() const;
	int32 GetMaxStackSize() const;

	FItemStack SplitStack(int32 Amount);

	void Shrink(int32 Quantity);

	void Grow(int32 Quantity);

	bool operator==(const FItemStack& Other) const;

	bool operator!=(const FItemStack& Other) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties)
	int32 StackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties)
	FGameplayTag ItemID;
};
