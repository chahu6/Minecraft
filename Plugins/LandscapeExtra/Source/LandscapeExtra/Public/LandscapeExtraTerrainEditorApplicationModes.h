#pragma once

#include "CoreMinimal.h"

struct LANDSCAPEEXTRA_API FLandscapeExtraTerrainEditorApplicationModes
{
	static const FName NoiseMode;
	static const FName ErosionMode;

	static FText GetLocalizedMode(const FName InMode);

private:
	FLandscapeExtraTerrainEditorApplicationModes() = delete;
};