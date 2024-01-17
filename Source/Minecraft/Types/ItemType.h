#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None", ToolTip = "��"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock", ToolTip = "��������"),
	NaturalBlock	UMETA(DisplayName = "NaturalBlock", ToolTip = "��Ȼ����"),
	Tool			UMETA(DisplayName = "Tool", ToolTip = "����"),
	Combat			UMETA(DisplayName = "Combat", ToolTip = "ս������"),
	Consumable		UMETA(DisplayName = "Consumable", ToolTip = "����Ʒ"),
};