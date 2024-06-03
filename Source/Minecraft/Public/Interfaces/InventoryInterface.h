#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

struct FItemStack;

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
	//virtual void TransferSlots(int32 SourceIndex, IInventoryInterface* SourceInventory, int32 DestinationIndex) = 0;
	virtual bool AddItemToInventory(FItemStack& ItemStack) = 0;
};
