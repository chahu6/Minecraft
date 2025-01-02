// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Gen/WorldGenAbstractTree.h"
#include "World/Block/Block.h"
#include "MinecraftGameplayTags.h"

bool UWorldGenAbstractTree::CanGrowInto(const UBlock* InBlock) const
{
    TArray<FGameplayTag> Tags;
    Tags.Add(FMinecraftGameplayTags::Get().Air);
    Tags.Add(FMinecraftGameplayTags::Get().Grass);
    Tags.Add(FMinecraftGameplayTags::Get().Dirt);
    Tags.Add(FMinecraftGameplayTags::Get().Log);
    return InBlock->BlockID.MatchesAnyExact(FGameplayTagContainer::CreateFromArray(Tags));
}
