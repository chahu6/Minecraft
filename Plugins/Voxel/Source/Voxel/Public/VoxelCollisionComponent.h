// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "VoxelCollisionComponent.generated.h"

/**
 * 
 */
UCLASS()
class VOXEL_API UVoxelCollisionComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
public:
	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	// End UPrimitiveComponent interface.

	// Begin USceneComponent interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual class UBodySetup* GetBodySetup() override;
	// End USceneComponent interface.

private:
	UPROPERTY(transient, duplicatetransient)
	TObjectPtr<class UBodySetup> BodySetup;
};
