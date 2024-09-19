#pragma once

#include "CoreMinimal.h"
#include "FaceType.generated.h"

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Forward,
	Background,
	Left,
	Right,
	Up,
	Down
};