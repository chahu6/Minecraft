#include "Init/Blocks.h"
#include "MinecraftGameplayTags.h"

#define REGISTER_BLOCKS(BlockName) \
	BlockName = GetRegisteredBlock(GameplayTags.Item_Block_##BlockName)

UBlocks* UBlocks::SingletonManager = nullptr;

const UBlock* UBlocks::Air;
const UBlock* UBlocks::Stone;
const UBlock* UBlocks::Grass;
const UBlock* UBlocks::Dirt;
const UBlock* UBlocks::Bedrock;
const UBlock* UBlocks::Water;
const UBlock* UBlocks::Sand;
const UBlock* UBlocks::Tallgrass;
const UBlock* UBlocks::Rose;
const UBlock* UBlocks::Log;
const UBlock* UBlocks::Planks;
const UBlock* UBlocks::CraftingTable;
const UBlock* UBlocks::Leaves;

UBlocks::~UBlocks()
{
	SingletonManager = nullptr;
}

void UBlocks::Initialization()
{
	SingletonManager = NewObject<UBlocks>(GetTransientPackage(), NAME_None);
	SingletonManager->AddToRoot();

	const FMinecraftGameplayTags& GameplayTags = FMinecraftGameplayTags::Get();

	Air = GetRegisteredBlock(GameplayTags.Air);
	REGISTER_BLOCKS(Stone);
	REGISTER_BLOCKS(Dirt);
	REGISTER_BLOCKS(Grass);
	REGISTER_BLOCKS(Bedrock);
	REGISTER_BLOCKS(Water);
	REGISTER_BLOCKS(Sand);
	REGISTER_BLOCKS(Tallgrass);
	REGISTER_BLOCKS(Rose);
	REGISTER_BLOCKS(Log);
	REGISTER_BLOCKS(Planks);
	REGISTER_BLOCKS(CraftingTable);
	REGISTER_BLOCKS(Leaves);
}

const UBlock* UBlocks::GetRegisteredBlock(const FGameplayTag& InBlockID)
{
	if (UBlock::REGISTER_NAME.Contains(InBlockID))
	{
		return UBlock::REGISTER_NAME[InBlockID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Block Requested: %s"), *InBlockID.ToString());
	}
	return nullptr;
}
