// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEntity/TileEntity.h"
#include "Item/ItemStack.h"
#include "TileEntityChest.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API ATileEntityChest : public ATileEntity
{
	GENERATED_BODY()
public:
	ATileEntityChest();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	TArray<FItemStack> ChestContents;
};
