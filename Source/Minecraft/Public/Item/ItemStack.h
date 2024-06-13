#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemStack.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TObjectPtr<UItem> Item = nullptr;

	void Clear()
	{
		ID = -1;
		Quantity = 0;
		Item = nullptr;
	}

	bool IsEmpty()
	{
		return ID < 0;
	}

	bool IsStack()
	{
		return Item ? Item->IsStack() : false;
	}

	int32 GetMaxCount() const 
	{
		return Item ? Item->GetMaxCount() : 0;
	}
};
