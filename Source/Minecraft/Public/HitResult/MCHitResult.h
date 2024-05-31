#pragma once

#include "CoreMinimal.h"

enum class EHitResultTpye : uint8
{
	MISS,
	BLOCK,
	ENTITY
};

/**
 * 
 */
struct FMCHitResult
{
	virtual EHitResultTpye GetType() = 0;
};
