// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Item.generated.h"

class UBlock;
class AEntityPlayer;
class AWorldManager;

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

	static UItem* GetItemFromBlock(const UBlock* Block);

	static UItem* GetItemFromName(const FGameplayTag& TagName);

	static UItem* GetItemFromName(const FName& ItemName);

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

public:
	virtual bool OnItemUse(AEntityPlayer* Player, AWorldManager* WorldManager, const FIntVector& BlockVoxelLocation, const FVector& HitNormal);

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
	bool bIsStack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "64", EditCondition = "bIsStack"), Category = Property)
	int32 MaxStackSize = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	FText Discription = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<UTexture2D> Icon;
};
