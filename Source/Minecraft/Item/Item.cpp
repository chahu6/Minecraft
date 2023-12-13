#include "Item.h"

TMap<TSharedPtr<FBlock>, TSharedPtr<FItem>> FItem::BLOCK_ITEM;

FItem::FItem(Properties properties)
{
	this->maxDamage = properties.maxDamage;
	this->maxStackSize = properties.maxStackSize;
	this->foodProperties = properties.foodProperties;
}