// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TileEntityProvider.generated.h"

class ATileEntity;
class AWorldManager;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTileEntityProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFT_API ITileEntityProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ATileEntity* CreateNewTileEntity(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation) = 0;
};
