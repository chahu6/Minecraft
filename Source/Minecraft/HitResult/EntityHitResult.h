#pragma once

#include "CoreMinimal.h"
#include "MCHitResult.h"

struct MINECRAFT_API FEntityHitResult : public FMCHitResult
{
	virtual EHitResultTpye GetType() override { return EHitResultTpye::ENTITY; }
};