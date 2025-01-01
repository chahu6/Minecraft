#pragma once

#include "CoreMinimal.h"
#include "BlockRenderType.generated.h"

UENUM(BlueprintType)
enum class EBlockRenderType : uint8
{
	Surface,
	Model,
	Skeletal
};