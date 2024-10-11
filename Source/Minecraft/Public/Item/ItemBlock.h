// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "ItemBlock.generated.h"

class UBlock;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UItemBlock : public UItem
{
	GENERATED_BODY()
public:
	UBlock* GetBlock() const;

	virtual bool OnItemUse(AEntityPlayer* Player, AWorldManager* WorldManager, const FIntVector& BlockVoxelLocation, const FVector& HitNormal) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TObjectPtr<UBlock> Block;
};
