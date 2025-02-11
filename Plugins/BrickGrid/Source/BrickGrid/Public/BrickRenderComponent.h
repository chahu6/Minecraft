// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "BrickRenderComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class BRICKGRID_API UBrickRenderComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
public:
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	UPROPERTY(EditAnywhere)
	FColor WireframeColor;
};
