#pragma once

#define GetLocationFromIndex(x)			(((x) * 100) - 50.0f)
#define GetBlocksIndex(x, y, z)			((x) + (y * 16) + (z * 256))

#define GetHeightIndex(x, y)			((x) + (y * 16))
