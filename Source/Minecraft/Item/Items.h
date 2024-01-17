#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Minecraft/Block/Blocks.h"
#include "BlockItem.h"
#include "AirItem.h"

struct Items
{
	static TSharedPtr<FItem> RegisterBlock(TSharedPtr<FBlock> Block);

	static TSharedPtr<FItem> RegisterBlock(TSharedPtr<FBlockItem> BlockItem);

	static TSharedPtr<FItem> RegisterBlock(TSharedPtr<FBlock> Block, TSharedPtr<FItem> Item);

	static TSharedPtr<FItem> RegisterItem();

	static TSharedPtr<FItem> AIR;
	static TSharedPtr<FItem> STONE;
	static TSharedPtr<FItem> DIRT;
	static TSharedPtr<FItem> GRASS;
	static TSharedPtr<FItem> IRON_HELMET;
};
