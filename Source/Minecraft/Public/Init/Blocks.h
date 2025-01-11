#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "World/Block/Block.h"
#include "Blocks.generated.h"

struct FGameplayTag;

UCLASS()
class MINECRAFT_API UBlocks : public UObject
{
	GENERATED_BODY()
public:
	static const UBlock* Air;
	static const UBlock* Stone;
	static const UBlock* Grass;
	static const UBlock* Dirt;
	static const UBlock* Bedrock;
	static const UBlock* Water;
	static const UBlock* Sand;
	static const UBlock* Tallgrass;
	static const UBlock* Rose;
	static const UBlock* Log;
	static const UBlock* Planks;
	static const UBlock* Workbench;
	static const UBlock* Leaves;

public:
	~UBlocks();

	FORCEINLINE static UBlocks& Get()
	{
		if (SingletonManager == nullptr)
		{
			Initialization();
		}
		return *SingletonManager;
	}

	static void Initialization();

private:
	static const UBlock* GetRegisteredBlock(const FGameplayTag& InBlockID);

private:
	static UBlocks* SingletonManager;
};