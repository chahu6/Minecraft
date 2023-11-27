#pragma once

#include "CoreMinimal.h"
#include "FaceType.generated.h"

UENUM(BlueprintType)
enum class EFaceType : uint8
{
	Forward,
	BackGround,
	Left,
	Right,
	Up,
	Down,

	EFT_MAX
};