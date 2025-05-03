// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelRenderComponent.h"

/** Vertex Buffer */
class FVoxelChunkVertexBuffer : public FVertexBuffer
{
public:

};

/** Index Buffer */
class FVoxelChunkIndexBuffer : public FIndexBuffer
{
public:

};

class FVoxelChunkVertexFactory : public FLocalVertexFactory
{
public:
	FVoxelChunkVertexFactory(ERHIFeatureLevel::Type InFeatureLevel)
		: FLocalVertexFactory(InFeatureLevel, "FVoxelChunkVertexFactory")
	{ }
};

/** Scene proxy */
class FVoxelChunkSceneProxy : public FPrimitiveSceneProxy
{
public:
	FVoxelChunkVertexBuffer VertexBuffer;
	FVoxelChunkIndexBuffer IndexBuffer;
	FVoxelChunkVertexFactory VertexFactories[6];

	FVoxelChunkSceneProxy(UVoxelRenderComponent* Component)
		: FPrimitiveSceneProxy(Component),
		VertexFactories{GetScene().GetFeatureLevel(), GetScene().GetFeatureLevel(), GetScene().GetFeatureLevel(), GetScene().GetFeatureLevel(), GetScene().GetFeatureLevel(), GetScene().GetFeatureLevel() }
	{}

	virtual uint32 GetMemoryFootprint(void) const { return(sizeof(*this) + GetAllocatedSize()); }
	SIZE_T GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

	SIZE_T GetTypeHash() const override 
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}
};

FPrimitiveSceneProxy* UVoxelRenderComponent::CreateSceneProxy()
{
	return new FVoxelChunkSceneProxy(this);
}

void UVoxelRenderComponent::GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const
{

}

FBoxSphereBounds UVoxelRenderComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = LocalToWorld.GetLocation();
	NewBounds.BoxExtent = FVector(1000.0f, 1000.0f, 1000.0f);
	NewBounds.SphereRadius = 1000.0f;
	return NewBounds;
}
