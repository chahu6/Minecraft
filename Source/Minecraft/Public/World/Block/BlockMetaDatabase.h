#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "BlockMetaDatabase.generated.h"

class UBlockBehavior;

UCLASS()
class UBlockMetaDatabase : public UObject
{
	GENERATED_BODY()
public:
	~UBlockMetaDatabase();

	//static void RegisterBlock(int32 ID, IBlock* Block);
	FORCEINLINE static UBlockMetaDatabase& Get()
	{
		if (SingletonManager == nullptr)
		{
			Initializer();
		}
		return *SingletonManager;
	}

private:
	static void Initializer();

	static UBlockMetaDatabase* SingletonManager;
};