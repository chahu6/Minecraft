#pragma once

#include "DynamicMeshBuilder.h"

class FVoxelProcMeshBuffersRenderData;

struct FVoxelProcMeshBuffers
{
	/** Vertex buffer for this section */
	FStaticMeshVertexBuffers VertexBuffers;

	FDynamicMeshIndexBuffer32 IndexBuffer;

	/** Local bounds of this section */
	FBox LocalBounds = FBox(ForceInit);

	void UpdateStats();

	mutable TWeakPtr<FVoxelProcMeshBuffersRenderData> RenderData;
};