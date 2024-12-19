#pragma once

// #include "World/WorldGenerator.h"

namespace WorldGenerator
{
	// 一个Chunk的单位尺寸
	constexpr int32 CHUNK_SIZE = 16;
	constexpr int32	CHUNK_HEIGHT = 256;
	constexpr int32	CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
	constexpr int32 CHUNK_SECTION_VOLUME = CHUNK_AREA * CHUNK_SIZE;
	constexpr int32 CHUNK_VOLUME = CHUNK_AREA * CHUNK_HEIGHT;

	constexpr float	CHUNK_SPHERE_RADIUS = CHUNK_SIZE * 0.5f * 1.7321f;

	// 世界深度
	constexpr int32 WORLD_DEPTH = 256;

	// 手能伸长的最大长度
	constexpr int32 MAX_RAY_DIST = 5;

	// 一个方块的单位尺寸
	constexpr int32 BlockSize = 100;

	// 一个Chunk的尺寸
	constexpr int32 ChunkSize = CHUNK_SIZE * BlockSize;
	constexpr int32 ChunkSize_Half = ChunkSize / 2;

	// 进剪切平面
	constexpr int32 NearClipPlane = 10;

	// 温度&湿度
	constexpr float TEMP_COLD = 0.3f;
	constexpr float TEMP_WARM = 0.7f;
	constexpr float TEMP_HOT = 1.0f;
	constexpr float HUMIDITY_DRY = 0.3f;
	constexpr float HUMIDITY_WET = 1.0f;

	// 海平面
	constexpr int32 SEA_LEVEL = 30;

	// 指定一个世界的最低地表高度
	constexpr int32 SURFACE_HEIGHT = 20;
}

// 一些工具宏函数
// #define GetLocationFromIndex(x)			((x) * WorldSettings::BlockSize)
// #define GetBlocksIndex(x, y, z)			((x) + (y * WorldSettings::CHUNK_SIZE) + (z * WorldSettings::CHUNK_AREA))
// #define GetHeightIndex(x, y)			((x) + (y * WorldSettings::CHUNK_SIZE))