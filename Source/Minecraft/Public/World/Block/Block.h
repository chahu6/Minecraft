// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "World/Data/BlockState.h"
#include "Block.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MINECRAFT_API UBlock : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBlock();

	static TMap<FName, UBlock*> Registry;

	static void Initializer();

	virtual void RandomTick();

	virtual void UpdateTick();

	virtual void OnDestroy(const FVector& WorldLocation);

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	FBlockState GetDefaultBlockState() const;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 BlockID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	bool bFullBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 LightOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	bool bTranslucent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	float BlockHardness;

	/** 可以抵抗爆炸的程度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	float BlockResistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	int32 TickRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<USoundBase> PlaceSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<USoundBase> DestroySound;

private:
	FBlockState DefaultBlockState;

private:
	static void RegisterBlock(const FName& Name, UBlock* Block);
};
