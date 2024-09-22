// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "MinecraftAssetManager.h"
#include "Item/ItemBlock.h"

TMap<FName, UItem*> UItem::REGISTER;
TMap<UBlock*, UItem*> UItem::BLOCK_TO_ITEM;

UItem::UItem()
{
	ItemType = UMinecraftAssetManager::ItemType;
}

void UItem::RegisterItems()
{
	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<UObject*> ObjectList;
	AssetManager.GetPrimaryAssetObjectList(UMinecraftAssetManager::ItemType, ObjectList);

	for (UObject* Object : ObjectList)
	{
		if (UItem* Item = Cast<UItem>(Object))
		{
			if (Item->IsA<UItemBlock>()) // Item->GetClass()->IsChildOf<UItemBlock>()
			{
				UItemBlock* ItemBlock = Cast<UItemBlock>(Item);
				RegisterItemBlock(ItemBlock->GetBlock(), Item);
			}
			else
			{
				RegisterItem(Item->Tag.GetTagName(), Item);
			}
		}
	}
}

UItem* UItem::GetItemFromBlock(UBlock* Block)
{
	if (BLOCK_TO_ITEM.Contains(Block))
	{
		UItem* Item = BLOCK_TO_ITEM[Block];
		return Item;
	}
	return nullptr;
}

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(ItemType, GetFName());
}

FString UItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

void UItem::RegisterItemBlock(UBlock* Block, UItem* Item)
{
	RegisterItem(Item->Tag.GetTagName(), Item);
	BLOCK_TO_ITEM.Add(Block, Item);
}

void UItem::RegisterItem(const FName& Name, UItem* Item)
{
	REGISTER.Add(Name, Item);
}
