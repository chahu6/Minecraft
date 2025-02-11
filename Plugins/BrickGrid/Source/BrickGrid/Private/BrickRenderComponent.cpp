// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickRenderComponent.h"
#include "BrickChunkSceneProxy.h"

FPrimitiveSceneProxy* UBrickRenderComponent::CreateSceneProxy()
{
	return new FBrickChunkSceneProxy(this);
}

FBoxSphereBounds UBrickRenderComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(1000000.0f, 1000000.0f, 1000000.0f), 1000000.0f);
}
