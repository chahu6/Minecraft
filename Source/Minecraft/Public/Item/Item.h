// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Item.generated.h"

class UBlock;

/**
 * 
 */
UCLASS(BlueprintType)
class MINECRAFT_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UItem();

	static TMap<FName, UItem*> REGISTER;
	static TMap<UBlock*, UItem*> BLOCK_TO_ITEM;

	static void RegisterItems();

	static UItem* GetItemFromBlock(UBlock* Block);

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

private:
	static void RegisterItemBlock(UBlock* Block, UItem* Item);
	static void RegisterItem(const FName& Name, UItem* Item);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 MaxDamage;
};
