#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None", ToolTip = "��"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock", ToolTip = "������װ�ν����ķ���"),
	Tool			UMETA(DisplayName = "Tool", ToolTip = "����"),
	Weapon			UMETA(DisplayName = "Weapon", ToolTip = "����"),
	Armor			UMETA(DisplayName = "Armor", ToolTip = "����"),
	Food			UMETA(DisplayName = "Food", ToolTip = "ʳ��"),
	Miscellaneous	UMETA(DisplayName = "Miscellaneous", ToolTip = "����"),

	ET_Max
};