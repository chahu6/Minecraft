#pragma once

// һ��Chunk�ĵ�λ�ߴ�
constexpr int32 CHUNK_SIZE		= 16,
				CHUNK_HEIGHT	= 256,
				CHUNK_AREA		= CHUNK_SIZE * CHUNK_SIZE,
				CHUNK_SECTION_VOLUME = CHUNK_AREA * CHUNK_SIZE,
				CHUNK_VOLUME		 = CHUNK_AREA * CHUNK_HEIGHT;

constexpr float	CHUNK_SPHERE_RADIUS = CHUNK_SIZE * 0.5f * 1.7321f;

constexpr int32 MAX_QUEUE_SIZE  =	1;

// �����쳤����󳤶�
constexpr int32 MAX_RAY_DIST	=	5;

// һ������ĵ�λ�ߴ�
constexpr int32 BlockSize		=	100;

// һ��Chunk�ĳߴ�
constexpr int32 ChunkSize		=	CHUNK_SIZE * BlockSize,
				ChunkSize_Half  =	ChunkSize / 2;

// ������ƽ��
constexpr int32 NearClipPlane	=	10;

// ��ƽ��
constexpr int32 WATER_LEVEL		=	90;

// ����߶�
constexpr int32 WORLD_HEIGHT	=	16;		// ��

// һЩ���ߺ꺯��
#define GetLocationFromIndex(x)			((x) * BlockSize)
#define GetBlocksIndex(x, y, z)			((x) + (y * CHUNK_SIZE) + (z * CHUNK_AREA))
#define GetHeightIndex(x, y)			((x) + (y * CHUNK_SIZE))