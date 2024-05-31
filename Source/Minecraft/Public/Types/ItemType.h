#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None", ToolTip = "无"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock", ToolTip = "建筑方块"),
	NaturalBlock	UMETA(DisplayName = "NaturalBlock", ToolTip = "自然方块"),
	Tool			UMETA(DisplayName = "Tool", ToolTip = "工具"),
	Combat			UMETA(DisplayName = "Combat", ToolTip = "战斗工具"),
	Consumable		UMETA(DisplayName = "Consumable", ToolTip = "消耗品"),
};