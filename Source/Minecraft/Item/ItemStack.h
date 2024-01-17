#pragma once

#include "CoreMinimal.h"
#include "Minecraft/Types/ItemType.h"
#include "ItemStack.generated.h"

class FItem;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_USTRUCT_BODY()

	FItemStack();
	FItemStack(int32 ID, TSharedPtr<FItem> NewItem, int32 Amount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 MaxCount = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	EItemType Type = EItemType::None;

	TSharedPtr<FItem> Item = nullptr;

	void Clear();

	bool IsEmpty();
};
