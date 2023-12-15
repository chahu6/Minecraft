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

	FItemStack() = default;
	FItemStack(int32 ID, TSharedPtr<FItem> NewItem, int32 Amount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 MaxCount = 64;

	TSharedPtr<FItem> Item = nullptr;

	void Empty();
};
