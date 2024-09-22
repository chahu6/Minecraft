// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Block/Block.h"
#include "MinecraftAssetManager.h"
#include "Kismet/GameplayStatics.h"

TMap<FName, UBlock*> UBlock::Registry;

UBlock::UBlock()
{
	ItemType = UMinecraftAssetManager::BlockType;
	DefaultBlockState.BlockID = BlockID;
	DefaultBlockState.SetBlock(this);
}

void UBlock::RandomTick()
{
	this->UpdateTick();
}

void UBlock::UpdateTick()
{
}

void UBlock::OnDestroy(const FVector& WorldLocation)
{
	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound, WorldLocation);
	}
}

void UBlock::OnBlockClicked(AWorldManager* WorldManager, const FIntVector& BlockVoxelLoc, AEntityPlayer* Player)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Block Is Clicked")));
}

FPrimaryAssetId UBlock::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(ItemType, GetFName());
}

FBlockState UBlock::GetDefaultBlockState() const
{
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

void UBlock::RegisterBlocks()
{
	UMinecraftAssetManager& AssetManager = UMinecraftAssetManager::Get();

	TArray<UObject*> ObjectList;
	AssetManager.GetPrimaryAssetObjectList(UMinecraftAssetManager::BlockType, ObjectList);

	for (UObject* Object : ObjectList)
	{
		if (UBlock* Block = Cast<UBlock>(Object))
		{
			RegisterBlock(Block->Tag.GetTagName(), Block);
		}
	}
}

void UBlock::RegisterBlock(const FName& Name, UBlock* Block)
{
	//if (!Registry.Contains(Name))
	{
		Registry.Add(Name, Block);
	}
}
