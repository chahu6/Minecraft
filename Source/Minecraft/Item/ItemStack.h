#pragma once

#include "CoreMinimal.h"
#include "ItemStack.generated.h"

class FItem;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_USTRUCT_BODY()
public:
	FItemStack() = default;
	FItemStack(int32 ID, TSharedPtr<FItem> NewItem, int32 Amount);

	UPROPERTY()
	int32 ID = -1;

	TSharedPtr<FItem> Item = nullptr;

	UPROPERTY()
	int32 Amount = 1;
};
