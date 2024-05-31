#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Block/Block.h"

class MINECRAFT_API FBlockItem : public FItem
{
public:
	explicit FBlockItem(const TSharedPtr<FBlock>& NewBlock, const FItem::Properties& properties);

	//void RegisterBlocks(const TMap<TSharedPtr<Block>> )

	TSharedPtr<FBlock> GetBlock();

	static TSharedPtr<FItem> GetItemFromBlock(TSharedPtr<FBlock> blockIn);

	TSharedPtr<FItem> RegisterBlocks(TMap<TSharedPtr<FBlock>, TSharedPtr<FItem>>& BlockToItemMap, TSharedPtr<FItem> item);

	virtual EType GetItemType() const override { return EType::BlockItem; }

private:
	TSharedPtr<FBlock> Block;
};