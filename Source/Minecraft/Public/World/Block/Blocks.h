#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "World/Block/Block.h"
#include "Blocks.generated.h"

struct FGameplayTag;

UCLASS()
class UBlocks : public UObject
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

public:
	~UBlocks();

	//static void RegisterBlock(int32 ID, IBlock* Block);
	FORCEINLINE static UBlocks& Get()
	{
		if (SingletonManager == nullptr)
		{
			Initializer();
		}
		return *SingletonManager;
	}

	static void Initializer();

private:
	static UBlock* GetRegisteredBlock(const FName& Name);

	static UBlock* GetRegisteredBlock(const FGameplayTag& Tag);

private:
	static UBlocks* SingletonManager;
};