#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

class ADroppedItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFT_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual bool AddItem(const ADroppedItem* Item) = 0;
};
