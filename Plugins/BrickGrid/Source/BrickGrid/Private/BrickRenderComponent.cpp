// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickRenderComponent.h"
#include "BrickChunkSceneProxy.h"
#include "VoxelProcMeshBuffers.h"

FPrimitiveSceneProxy* UBrickRenderComponent::CreateSceneProxy()
{
	return new FBrickChunkSceneProxy(this);
}

FBoxSphereBounds UBrickRenderComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	//return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(1000000.0f, 1000000.0f, 1000000.0f), 1000000.0f);
	return FBoxSphereBounds(LocalBounds.TransformBy(LocalToWorld));
}

UBrickRenderComponent::UBrickRenderComponent()
{
	Mobility = EComponentMobility::Movable;
	CastShadow = true;

	TUniquePtr<FVoxelProcMeshBuffers> Buffers = MakeUnique<FVoxelProcMeshBuffers>();

	TArray<FDynamicMeshVertex> Vertices;

	FDynamicMeshVertex V0, V1, V2, V3;
	V0.Position = (FVector3f)FVector(0, 0, 0);
	V1.Position = (FVector3f)FVector(100, 0, 0);
	V2.Position = (FVector3f)FVector(0, 100, 0);
	V3.Position = (FVector3f)FVector(100, 100, 0);
	int32 I0 = Vertices.Add(V0);
	int32 I1 = Vertices.Add(V1);
	int32 I2 = Vertices.Add(V2);
	int32 I3 = Vertices.Add(V3);

	Buffers->IndexBuffer.Indices.Add(0);
	Buffers->IndexBuffer.Indices.Add(2);
	Buffers->IndexBuffer.Indices.Add(1);
	Buffers->IndexBuffer.Indices.Add(1);
	Buffers->IndexBuffer.Indices.Add(2);
	Buffers->IndexBuffer.Indices.Add(3);

	uint32 NumTexCoords = 1;

	if (Vertices.Num())
	{
		Buffers->VertexBuffers.PositionVertexBuffer.Init(Vertices.Num());
		Buffers->VertexBuffers.StaticMeshVertexBuffer.Init(Vertices.Num(), NumTexCoords);
		Buffers->VertexBuffers.ColorVertexBuffer.Init(Vertices.Num());

		for (int32 i = 0; i < Vertices.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			Buffers->VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			Buffers->VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector3f(), Vertex.GetTangentY(), Vertex.TangentZ.ToFVector3f());
			for (uint32 j = 0; j < NumTexCoords; j++)
			{
				Buffers->VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, j, Vertex.TextureCoordinate[j]);
			}
			Buffers->VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
		}
	}
	else
	{
		Buffers->VertexBuffers.PositionVertexBuffer.Init(1);
		Buffers->VertexBuffers.StaticMeshVertexBuffer.Init(1, 1);
		Buffers->VertexBuffers.ColorVertexBuffer.Init(1);

		Buffers->VertexBuffers.PositionVertexBuffer.VertexPosition(0) = FVector3f(0, 0, 0);
		Buffers->VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(0, FVector3f(1, 0, 0), FVector3f(0, 1, 0), FVector3f(0, 0, 1));
		Buffers->VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(0, 0, FVector2f(0, 0));
		Buffers->VertexBuffers.ColorVertexBuffer.VertexColor(0) = FColor(1, 1, 1, 1);
	}

	AddProcMeshSection(MoveTemp(Buffers));
}

void UBrickRenderComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	UPrimitiveComponent::OnComponentDestroyed(bDestroyingHierarchy);

	ProcMeshSections.Reset();
}

void UBrickRenderComponent::UpdateLocalBounds()
{
	FBox LocalBox(ForceInit);

	for (const FVoxelProcMeshSection& Section : ProcMeshSections)
	{
		LocalBox += Section.Buffers->LocalBounds;
	}

	LocalBounds = LocalBox.IsValid ? FBoxSphereBounds(LocalBox) : FBoxSphereBounds(ForceInit);

	// Update global bounds;
	UpdateBounds();
	// Need to send to render thread
	MarkRenderTransformDirty();
}

int32 UBrickRenderComponent::AddProcMeshSection(TUniquePtr<FVoxelProcMeshBuffers> Buffers)
{
	check(Buffers.IsValid());

	/*if (Buffers->GetNumIndices() == 0)
	{
		return -1;
	}*/

	const int32 Index = ProcMeshSections.Emplace();
	SetProcMeshSection(Index, MoveTemp(Buffers));

	return int32();
}

void UBrickRenderComponent::SetProcMeshSection(int32 Index, TUniquePtr<FVoxelProcMeshBuffers> Buffers)
{
	if (!ensure(ProcMeshSections.IsValidIndex(Index)))
	{
		return;
	}

	Buffers->UpdateStats();

	ProcMeshSections[Index].Buffers = MakeShareable(Buffers.Release());

}
