#pragma once

#include "CoreMinimal.h"

enum class EType : uint8
{
	None,
	Item,
	BlockItem
};

class IItemType
{
public:
	virtual ~IItemType() = default;

	virtual EType GetItemType() const = 0;
};