// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/Block.h"
#include "MinecraftAssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Item.h"
#include "Init/Items.h"
#include "World/WorldManager.h"
#include "Item/ItemStack.h"

TMap<FGameplayTag, const UBlock*> UBlock::REGISTER_NAME;

UBlock::UBlock()
{
	PrimaryAssetType = UMinecraftAssetManager::BlockType;
}

void UBlock::RandomTick()
{
	this->UpdateTick();
}

void UBlock::UpdateTick()
{
}

const UBlock* UBlock::GetBlockByID(const FGameplayTag& InBlockID)
{
	if (REGISTER_NAME.Contains(InBlockID))
	{
		return REGISTER_NAME[InBlockID];
	}
	check(false);
	return nullptr;
}

void UBlock::OnBlockDestroyedByPlayer(AWorldManager* WorldManager, const FVector& WorldLocation)
{
	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(WorldManager, DestroySound, WorldLocation);
	}
}

int32 UBlock::QuantityDroppedWithBonus(int32 Forture)
{
	return QuantityDropped();
}

int32 UBlock::QuantityDropped()
{
	return 1;
}

void UBlock::DropBlockAsItem(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation, int32 Forture)
{
	DropBlockAsItemWithChance(WorldManager, BlockWorldVoxelLocation, 1.f, Forture);
}

void UBlock::DropBlockAsItemWithChance(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation, float Chance, int32 Forture)
{
	int32 Number = QuantityDroppedWithBonus(Forture);

	for (int32 i = 0; i < Number; ++i)
	{
		if (FMath::FRand() <= Chance)
		{
			UItem* Item = GetItemDropped(Forture);
			if (Item != UItems::Air)
			{
				FItemStack ItemStack;
				ItemStack.SetItemID(Item->ItemID);
				ItemStack.SetCount(1);
				SpawnAsEntity(WorldManager, BlockWorldVoxelLocation, ItemStack);
			}
		}
	}
}

void UBlock::OnBlockPlacedBy()
{

}

EBlockRenderType UBlock::GetRenderType() const
{
	return EBlockRenderType::Surface;
}

UItem* UBlock::GetItemDropped(int32 Forture)
{
	return UItem::GetItemFromBlock(this);
}

void UBlock::OnBlockClicked(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Block Is Clicked")));
}

bool UBlock::OnBlockActivated(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	return false;
}

FItemStack UBlock::GetItem()
{
	return FItemStack();
}

FPrimaryAssetId UBlock::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(PrimaryAssetType, GetFName());
}

FBlockState UBlock::GetDefaultBlockState() const
{
	FBlockState DefaultBlockState;
	DefaultBlockState.SetBlock(this);
	return DefaultBlockState;
}

FString UBlock::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

float UBlock::GetPlayerRelativeBlockHardness()
{
	float f = GetBlockHardness();

	if (f > 0.f)
	{
		return 1.f / f / 30.f;
	}

	return 0.0f;
}

float UBlock::GetBlockHardness()
{
	return BlockHardness;
}

bool UBlock::IsFullBlock() const
{
	return bFullBlock;
}

AEntityItem* UBlock::SpawnAsEntity(AWorldManager* WorldManager, const FIntVector& BlockWorldVoxelLocation, const FItemStack& ItemStack)
{
	return WorldManager->SpawnEntity(BlockWorldVoxelLocation, ItemStack);
}

void UBlock::RegisterBlocks()
{
	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<UObject*> ObjectList;
	AssetManager.GetPrimaryAssetObjectList(UMinecraftAssetManager::BlockType, ObjectList);

	for (UObject* Object : ObjectList)
	{
		if (UBlock* Block = Cast<UBlock>(Object))
		{
			RegisterBlock(Block);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Block REGISTER_NAME Num: %d"), REGISTER_NAME.Num());
}

void UBlock::RegisterBlock(UBlock* Block)
{
	//if (!REGISTER_NAME.Contains(Block->BlockID))
	{
		REGISTER_NAME.Add(Block->BlockID, Block);
	}

	//REGISTER_ID.Add(Block->BlockID, Block);
}
