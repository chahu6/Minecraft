#include "Block/Block.h"
#include "Item/DroppedItem.h"
#include "Item/Item.h"
#include "Core/BlockPos.h"

TMap<int32, TSharedPtr<FBlock>> FBlock::BlockRegister;

FBlock::FBlock(const Properties& properties)
{
	this->Hardness = properties.DestroyTime;
	this->bIsAir = properties.bIsAir;
}

TSharedPtr<FBlock> FBlock::RegisterBlock(int32 BlockID, TSharedPtr<FBlock> NewBlock)
{
	if (!BlockRegister.Contains(BlockID))
	{
		return BlockRegister.Emplace(BlockID, NewBlock);
	}

	return BlockRegister[BlockID];
}

TSharedPtr<FBlock> FBlock::GetBlock(int32 BlockID)
{
	if (BlockRegister.Contains(BlockID))
	{
		return BlockRegister[BlockID];
	}

	return nullptr;
}

float FBlock::GetPlayerRelativeBlockHardness(const AMCPlayer* Player)
{
	float f = GetBlockHardness();
	return f < 0.0f ? 0.0f : 1.0f / (f * 1.5f);
}

void FBlock::DropBlockAsItem(UWorld* World, const FBlockPos& BlockPos, int32 BlockID) const
{
	if (CanDroppedItem())
	{
		TSharedPtr<FItem> Item = GetItemDropped(BlockID);
		if (Item)
		{
			if (World)
			{
				ADroppedItem* DroppedItem = World->SpawnActor<ADroppedItem>(ADroppedItem::StaticClass(), BlockPos.WorldLocation() + BlockSize / 2.0f, FRotator::ZeroRotator);
				DroppedItem->SetItemStack(FItemStack(BlockID, Item, 1));
			}
		}
	}
}

TSharedPtr<FItem> FBlock::GetItemDropped(int32 BlockID) const
{
	return FItem::BLOCK_ITEM[GetBlock(BlockID)];
}
