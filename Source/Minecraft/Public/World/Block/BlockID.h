#pragma once

#include "CoreMinimal.h"
#include "BlockID.generated.h"

UENUM(BlueprintType)
enum class EBlockID : uint8
{
	Air,
	Stone,
	Dirt,
	Grass,
	BedRock,
	Water,

	EBT_MAX
};