#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None", ToolTip = "无"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock", ToolTip = "建筑和装饰建筑的方块"),
	Tool			UMETA(DisplayName = "Tool", ToolTip = "工具"),
	Weapon			UMETA(DisplayName = "Weapon", ToolTip = "武器"),
	Armor			UMETA(DisplayName = "Armor", ToolTip = "防具"),
	Food			UMETA(DisplayName = "Food", ToolTip = "食物"),
	Miscellaneous	UMETA(DisplayName = "Miscellaneous", ToolTip = "杂项"),

	ET_Max
};