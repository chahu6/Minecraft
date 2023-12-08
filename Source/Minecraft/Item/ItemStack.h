#pragma once

#include "CoreMinimal.h"
#include "ItemStack.generated.h"

class Item;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_USTRUCT_BODY()
public:
	FItemStack() = default;
	FItemStack(Item* item, int32 Amount);

private:
	Item* item = nullptr;
	int32 count = 1;
};
