#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EBlockID : int32
{
	Air = 0,
	Stone = 1,
	Dirt = 2,
	Grass = 3,
	RedRock = 4,
	Water = 5,

	MAX
};