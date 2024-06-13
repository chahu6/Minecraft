// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BlockItem.h"

UItem* UBlockItem::Clone()
{
	UBlockItem* BlockItem = NewObject<UBlockItem>();
	BlockItem->ItemInfo = this->ItemInfo;
	return BlockItem;
}
