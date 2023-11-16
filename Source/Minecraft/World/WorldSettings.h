#pragma once

// һ��Chunk�ĵ�λ�ߴ�
constexpr int32 CHUNK_SIZE = 16,
			CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
			CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE,
			CHUNK = CHUNK_AREA * 256;


// �����쳤����󳤶�
constexpr int32 MAX_RAY_DIST = 5;

// һ������ĵ�λ�ߴ�
constexpr int32 BlockSize = 100;

// һ��Chunk�ĳߴ�
constexpr int32 ChunkSize = CHUNK_SIZE * BlockSize;

// ��ƽ��
constexpr int32 WATER_LEVEL = 64;

// ���糤������
constexpr int32 WORLD_W = 10,		// ��
				WORLD_H = 3,		// ��
				WORLD_D = WORLD_W,	// ��
				WORLD_AREA = WORLD_W * WORLD_D,
				WORLD_VOL = WORLD_AREA * WORLD_H;

// һЩ�꺯��
#define GetLocationFromIndex(x)			((x) * BlockSize)
#define GetBlocksIndex(x, y, z)			((x) + (y * CHUNK_SIZE) + (z * CHUNK_AREA))

#define GetHeightIndex(x, y)			((x) + (y * CHUNK_SIZE))