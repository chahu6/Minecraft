// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelCollisionComponent.h"

FPrimitiveSceneProxy* UVoxelCollisionComponent::CreateSceneProxy()
{
    return nullptr;
}

FBoxSphereBounds UVoxelCollisionComponent::CalcBounds(const FTransform& LocalToWorld) const
{
    return FBoxSphereBounds();
}

UBodySetup* UVoxelCollisionComponent::GetBodySetup()
{
    return BodySetup;
}
