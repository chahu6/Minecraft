#pragma once

// 一个Chunk的单位尺寸
constexpr int32 CHUNK_SIZE = 16,
			CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
			CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE,
			CHUNK = CHUNK_AREA * 256;


// 手能伸长的最大长度
constexpr int32 MAX_RAY_DIST = 5;

// 一个方块的单位尺寸
constexpr int32 BlockSize = 100;

// 一个Chunk的尺寸
constexpr int32 ChunkSize = CHUNK_SIZE * BlockSize;

// 海平面
constexpr int32 WATER_LEVEL = 64;

// 世界长、宽、高
constexpr int32 WORLD_W = 10,		// 宽
				WORLD_H = 3,		// 高
				WORLD_D = WORLD_W,	// 长
				WORLD_AREA = WORLD_W * WORLD_D,
				WORLD_VOL = WORLD_AREA * WORLD_H;

// 一些宏函数
#define GetLocationFromIndex(x)			((x) * BlockSize)
#define GetBlocksIndex(x, y, z)			((x) + (y * CHUNK_SIZE) + (z * CHUNK_AREA))

#define GetHeightIndex(x, y)			((x) + (y * CHUNK_SIZE))