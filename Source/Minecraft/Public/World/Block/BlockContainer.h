// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Block/Block.h"
#include "Interfaces/Block/TileEntityProvider.h"
#include "BlockContainer.generated.h"

class ATileEntity;

/**
 * 
 */
UCLASS(Abstract)
class MINECRAFT_API UBlockContainer : public UBlock, public ITileEntityProvider
{
	GENERATED_BODY()
public:
	/** Tile Entity Provider*/
	virtual ATileEntity* CreateNewTileEntity(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation) override;
	/** end Tile Entity Provider*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TSubclassOf<ATileEntity> TileEntityClass;
};
