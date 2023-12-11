#include "Item.h"

TMap<uint8, Item> Item::BLOCK_ITEM;

Item::Item(Properties properties)
{
	this->maxDamage = properties.maxDamage;
	this->maxStackSize = properties.maxStackSize;
	this->foodProperties = properties.foodProperties;
}

Item::~Item()
{
}
