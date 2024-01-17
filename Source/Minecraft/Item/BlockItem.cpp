#include "BlockItem.h"

FBlockItem::FBlockItem(const TSharedPtr<FBlock>& NewBlock, const FItem::Properties& properties)
	:FItem(properties), Block(NewBlock)
{
}

TSharedPtr<FBlock> FBlockItem::GetBlock()
{
	return Block;
}

TSharedPtr<FItem> FBlockItem::GetItemFromBlock(TSharedPtr<FBlock> blockIn)
{
	return TSharedPtr<FItem>();
}

TSharedPtr<FItem> FBlockItem::RegisterBlocks(TMap<TSharedPtr<FBlock>, TSharedPtr<FItem>>& BlockToItemMap, TSharedPtr<FItem> Item)
{
	BlockToItemMap.Emplace(GetBlock(), Item);

	return Item;
}
