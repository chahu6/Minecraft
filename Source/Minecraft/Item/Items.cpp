#include "Items.h"
#include "BlockItem.h"

TSharedPtr<FItem> Items::AIR = RegisterBlock(Blocks::AIR, MakeShared<FAirItem>(Blocks::AIR, FItem::Properties()));
TSharedPtr<FItem> Items::STONE = RegisterBlock(Blocks::STONE);
TSharedPtr<FItem> Items::DIRT = RegisterBlock(Blocks::DIRT);
TSharedPtr<FItem> Items::GRASS = RegisterBlock(Blocks::GRASS);
TSharedPtr<FItem> Items::IRON_HELMET = RegisterItem();

TSharedPtr<FItem> Items::RegisterBlock(TSharedPtr<FBlock> Block)
{
	return RegisterBlock(MakeShared<FBlockItem>(Block, FItem::Properties()));
}

TSharedPtr<FItem> Items::RegisterBlock(TSharedPtr<FBlockItem> BlockItem)
{
	return RegisterBlock(BlockItem->GetBlock(), BlockItem);
}

TSharedPtr<FItem> Items::RegisterBlock(TSharedPtr<FBlock> Block, TSharedPtr<FItem> Item)
{
	if (Item->GetItemType() == EType::BlockItem)
	{
		TSharedPtr<FBlockItem> BlockItem = StaticCastSharedPtr<FBlockItem>(Item);
		return BlockItem->RegisterBlocks(FItem::BLOCK_ITEM, Item);
	}

	return nullptr;
}

TSharedPtr<FItem> Items::RegisterItem()
{
	return TSharedPtr<FItem>();
}
