// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEntity/TileEntity.h"
#include "Item/ItemStack.h"
#include "TileEntityChest.generated.h"

class UInventoryComponent;
class UContainer;

/**
 * 
 */
UCLASS()
class MINECRAFT_API ATileEntityChest : public ATileEntity
{
	GENERATED_BODY()
public:
	ATileEntityChest();

	/** 当玩家右键单击该块时调用 */
	virtual void OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UContainer> ChestClass;
};
