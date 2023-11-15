#pragma once

constexpr int32 CHUNK_SIZE = 16,
			CHUNK_AREA = 16 * 16,
			CHUNK_VOLUME = 16 * 16 * 16,
			CHUNK = 16 * 16 * 256;

constexpr int32 BlockSize = 100;
constexpr int32 ChunkSize = CHUNK_SIZE * BlockSize;
constexpr int32 WATER_LEVEL = 64;