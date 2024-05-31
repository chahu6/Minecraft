#include "Item/AirItem.h"
#include "Block/Block.h"

FAirItem::FAirItem(const FBlock* NewBlock, FItem::Properties properties)
	:FItem(properties)
{
	block = NewBlock;
}

FAirItem::FAirItem(TSharedPtr<FBlock> NewBlock, FItem::Properties properties)
{
}
