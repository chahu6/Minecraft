#include "Item.h"

Item::Item(Properties properties)
{
	this->maxDamage = properties.maxDamage;
	this->maxStackSize = properties.maxStackSize;
	this->foodProperties = properties.foodProperties;
}

Item::~Item()
{
}
