// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Block/Block.h"
#include "Interfaces/Block/TileEntityProvider.h"
#include "BlockTileEntity.generated.h"

class ATileEntity;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBlockTileEntity : public UBlock, public ITileEntityProvider
{
	GENERATED_BODY()
public:
	/** Tile Entity Provider*/
	virtual ATileEntity* CreateNewTileEntity_Implementation(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation) override;
	/** end Tile Entity Provider*/

	virtual EBlockRenderType GetRenderType() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Property)
	TSubclassOf<ATileEntity> TileEntityClass;
};
