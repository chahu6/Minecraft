#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Block/Block.h"

class MINECRAFT_API FAirItem : public FItem
{
public:
	//AirItem(const TSharedPtr<Block> NewBlock, Item::Properties properties);
	FAirItem(const FBlock* NewBlock, FItem::Properties properties);
	FAirItem(TSharedPtr<FBlock> NewBlock, FItem::Properties properties);

private:
	const FBlock* block;
};