#pragma once

#include "CoreMinimal.h"
#include "BiomeID.generated.h"

UENUM(BlueprintType)
enum class EBiomeID : uint8
{
	Ocean				UMETA(DisplayName = "Ocean"),
	Forest				UMETA(DisplayName = "Forest"),
	Desert				UMETA(DisplayName = "Desert"),
	FlourishIceField	UMETA(DisplayName = "FlourishIceField"),
	BarrenIceField		UMETA(DisplayName = "BarrenIceField"),
	InlandForest		UMETA(DisplayName = "InlandForest"),
	InlandPlain			UMETA(DisplayName = "InlandPlain"),
	RainForest			UMETA(DisplayName = "RainForest"),
	Savanna				UMETA(DisplayName = "Savanna"),
};