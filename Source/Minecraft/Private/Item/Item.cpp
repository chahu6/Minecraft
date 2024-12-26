// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "MinecraftAssetManager.h"
#include "Item/ItemBlock.h"
#include "MinecraftGameplayTags.h"

TMap<FGameplayTag, UItem*> UItem::REGISTER;
TMap<UBlock*, UItem*> UItem::BLOCK_TO_ITEM;

UItem::UItem()
{
	PrimaryAssetType = UMinecraftAssetManager::ItemType;
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
				check(ItemBlock->GetBlock());
				RegisterItemBlock(ItemBlock->GetBlock(), Item);
			}
			else
			{
				RegisterItem(Item->ItemID, Item);
			}
		}
	}
}

UItem* UItem::GetItemFromBlock(const UBlock* Block)
{
	if (BLOCK_TO_ITEM.Contains(Block))
	{
		return BLOCK_TO_ITEM[Block];
	}
	ensure(false);
	return REGISTER[FMinecraftGameplayTags::Get().Air];
}

UItem* UItem::GetItemFromID(const FString& Name)
{
	return GetItemFromID(FGameplayTag::RequestGameplayTag(FName(Name)));
}

UItem* UItem::GetItemFromID(const FGameplayTag& InItemID)
{
	//check(REGISTER.Contains(ItemName));
	if (REGISTER.Contains(InItemID))
	{
		return REGISTER[InItemID];
	}
	return REGISTER[FMinecraftGameplayTags::Get().Air];
}

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(PrimaryAssetType, GetFName());
}

FString UItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

bool UItem::OnItemUse(AEntityPlayer* Player, AWorldManager* WorldManager, const FIntVector& BlockVoxelLocation, const FVector& HitNormal)
{
	return false;
}

void UItem::RegisterItemBlock(UBlock* Block, UItem* Item)
{
	RegisterItem(Item->ItemID, Item);
	BLOCK_TO_ITEM.Add(Block, Item);
}

void UItem::RegisterItem(const FGameplayTag& InItemID, UItem* Item)
{
	REGISTER.Add(InItemID, Item);
}
