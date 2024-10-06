#include "Init/Blocks.h"
#include "MinecraftGameplayTags.h"

#define REGISTER_BLOCKS(BlockName) \
	BlockName = GetRegisteredBlock(GameplayTags.Minecraft_##BlockName)

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

UBlocks::~UBlocks()
{
	SingletonManager = nullptr;
}

void UBlocks::Initialization()
{
	SingletonManager = NewObject<UBlocks>(GetTransientPackage(), NAME_None);
	SingletonManager->AddToRoot();

	const FMinecraftGameplayTags& GameplayTags = FMinecraftGameplayTags::Get();

	REGISTER_BLOCKS(Air);
	REGISTER_BLOCKS(Stone);
	REGISTER_BLOCKS(Dirt);
	REGISTER_BLOCKS(Grass);
	REGISTER_BLOCKS(Bedrock);
	REGISTER_BLOCKS(Water);
	REGISTER_BLOCKS(Sand);
	REGISTER_BLOCKS(Tallgrass);
	REGISTER_BLOCKS(Rose);
}

UBlock* UBlocks::GetRegisteredBlock(const FName& Name)
{
	if (UBlock::Registry.Contains(Name))
	{
		return UBlock::Registry[Name];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Invalid Block Requested: %s"), *Name.ToString());
	}
	return nullptr;
}

UBlock* UBlocks::GetRegisteredBlock(const FGameplayTag& Tag)
{
	return GetRegisteredBlock(Tag.GetTagName());
}
