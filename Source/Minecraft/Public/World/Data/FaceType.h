#pragma once

#include "CoreMinimal.h"
#include "FaceType.generated.h"

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Up,
	Down,
	Forward,
	Backward,
	Left,
	Right
};