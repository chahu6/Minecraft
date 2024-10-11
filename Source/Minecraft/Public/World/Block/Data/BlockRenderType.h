#pragma once

#include "CoreMinimal.h"
#include "BlockRenderType.generated.h"

UENUM()
enum class EBlockRenderType : uint8
{
	Surface,
	Model,
	Skeletal
};