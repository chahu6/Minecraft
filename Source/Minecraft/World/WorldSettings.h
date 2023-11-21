#pragma once

// 一个Chunk的单位尺寸
constexpr int32 CHUNK_SIZE		=	16,
				CHUNK_AREA		=	CHUNK_SIZE * CHUNK_SIZE,
				CHUNK_VOLUME	=	CHUNK_AREA * CHUNK_SIZE,
				CHUNK			=	CHUNK_AREA * 256;

constexpr float	CHUNK_SPHERE_RADIUS = CHUNK_SIZE * 0.5f * 1.7321f;

// 手能伸长的最大长度
constexpr int32 MAX_RAY_DIST	=	5;

// 一个方块的单位尺寸
constexpr int32 BlockSize		=	100;

// 一个Chunk的尺寸
constexpr int32 ChunkSize		=	CHUNK_SIZE * BlockSize,
				ChunkSize_Half  =	ChunkSize / 2;

// 进剪切平面
constexpr int32 NearClipPlane	=	10;

// 海平面
constexpr int32 WATER_LEVEL		=	64;

// 世界高度
constexpr int32 WORLD_HEIGHT	=	3;		// 高

// 一些工具宏函数
#define GetLocationFromIndex(x)			((x) * BlockSize)
#define GetBlocksIndex(x, y, z)			((x) + (y * CHUNK_SIZE) + (z * CHUNK_AREA))
#define GetHeightIndex(x, y)			((x) + (y * CHUNK_SIZE))