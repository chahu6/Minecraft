// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "World/Data/BlockState.h"
#include "Block.generated.h"

class AWorldManager;
class AEntityPlayer;
class UItem;
struct FItemStack;

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

	static void RegisterBlocks();

	virtual void RandomTick();

	virtual void UpdateTick();

	virtual void OnBlockDestroyedByPlayer(AWorldManager* WorldManager, const FVector& WorldLocation);

	virtual void DropBlockAsItemWithChance(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation, float Chance, int32 Forture);

	virtual void OnBlockPlacedBy();

	/**
	* ��ȡ���������ջ�ʱӦ�������Ʒ��
	*/
	virtual UItem* GetItemDropped(int32 Forture);

	/**
	* ���ݸ���������ˮƽ��ȡ�½�������
	*/
	virtual int32 QuantityDroppedWithBonus(int32 Forture);

	/**
	* ����Block����ʱҪ��������Ʒ����
	*/
	virtual int32 QuantityDropped();

public:
	void DropBlockAsItem(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation, int32 Forture);

	/** Block����� */
	virtual void OnBlockClicked(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player);

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	FBlockState GetDefaultBlockState() const;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	float GetPlayerRelativeBlockHardness();

	float GetBlockHardness();

protected:
	static void SpawnAsEntity(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation, const FItemStack& ItemStack);

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

	/** ���Եֿ���ը�ĳ̶� */
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
