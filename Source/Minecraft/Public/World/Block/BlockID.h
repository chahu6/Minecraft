#pragma once

#include "CoreMinimal.h"

using Block_t = int32;

UENUM()
enum class EBlockID : int32
{
	Air,
	Stone,
	Dirt,
	GrassBlock,
	BedRock,
	Water,
	Grass,
	Rose,
	Oak_Wood,
	Oak_Planks,

	MAX
};