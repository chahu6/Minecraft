#include "World/Block/Blocks.h"
#include "MinecraftGameplayTags.h"

#define REGISTER_BLOCK(BlockName) \
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

void UBlocks::Initializer()
{
	SingletonManager = NewObject<UBlocks>(GetTransientPackage(), NAME_None);
	SingletonManager->AddToRoot();

	const FMinecraftGameplayTags& GameplayTags = FMinecraftGameplayTags::Get();

	REGISTER_BLOCK(Air);
	REGISTER_BLOCK(Stone);
	REGISTER_BLOCK(Dirt);
	REGISTER_BLOCK(Grass);
	REGISTER_BLOCK(Bedrock);
	REGISTER_BLOCK(Water);
	REGISTER_BLOCK(Sand);
	REGISTER_BLOCK(Tallgrass);
	REGISTER_BLOCK(Rose);
}

UBlock* UBlocks::GetRegisteredBlock(const FName& Name)
{
	if (UBlock::Registry.Contains(Name))
	{
		return UBlock::Registry[Name];
	}
	return nullptr;
}

UBlock* UBlocks::GetRegisteredBlock(const FGameplayTag& Tag)
{
	return GetRegisteredBlock(Tag.GetTagName());
}
