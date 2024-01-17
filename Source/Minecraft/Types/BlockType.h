#pragma once

#include "CoreMinimal.h"
#include "BlockType.generated.h"

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	Air,
	Stone,
	Dirt,
	Grass,
	BedRock,
	Water,

	EBT_MAX
};