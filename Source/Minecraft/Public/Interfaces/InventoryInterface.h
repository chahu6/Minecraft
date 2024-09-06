#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

struct FItemData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFT_API IInventoryInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TryAddItem(int32 Index, UPARAM(ref) FItemData& InItemData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveItem(int32 Index, UPARAM(ref) FItemData& OutItemData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TransferItem(int32 Index, UPARAM(ref) FItemData& OutItemData);
};
