#pragma once

#include "CoreMinimal.h"
#include "Block.h"

#include "Minecraft/Item/AirItem.h"

struct Blocks
{
private:
	static TSharedPtr<FBlock> RegisterBlock(int32 BlockID, TSharedPtr<FBlock> NewBlock);

public:
	static TMap<int32, TSharedPtr<FBlock>> BlocksMap;

	static TSharedPtr<FBlock> AIR;
	static TSharedPtr<FBlock> STONE;
	static TSharedPtr<FBlock> DIRT;
	static TSharedPtr<FBlock> GRASS;
};
