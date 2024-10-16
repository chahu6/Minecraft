#pragma once

// #include "World/WorldSettings.h"

namespace WorldSettings
{
	// 一个Chunk的单位尺寸
	constexpr int32 CHUNK_SIZE = 16,
		CHUNK_HEIGHT = 256,
		CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
		CHUNK_SECTION_VOLUME = CHUNK_AREA * CHUNK_SIZE,
		CHUNK_VOLUME = CHUNK_AREA * CHUNK_HEIGHT;

	constexpr float	CHUNK_SPHERE_RADIUS = CHUNK_SIZE * 0.5f * 1.7321f;

	// 手能伸长的最大长度
	constexpr int32 MAX_RAY_DIST = 5;

	// 一个方块的单位尺寸
	constexpr int32 BlockSize = 100;

	// 一个Chunk的尺寸
	constexpr int32 ChunkSize = CHUNK_SIZE * BlockSize,
		ChunkSize_Half = ChunkSize / 2;

	// 进剪切平面
	constexpr int32 NearClipPlane = 10;

	// 海平面
	constexpr int32 WATER_LEVEL = 90;

	// 世界高度
	constexpr int32 WORLD_HEIGHT = 16;		// 高

	constexpr int32 SURFACE_HEIGHT = 20;
}

// 一些工具宏函数
// #define GetLocationFromIndex(x)			((x) * WorldSettings::BlockSize)
// #define GetBlocksIndex(x, y, z)			((x) + (y * WorldSettings::CHUNK_SIZE) + (z * WorldSettings::CHUNK_AREA))
// #define GetHeightIndex(x, y)			((x) + (y * WorldSettings::CHUNK_SIZE))