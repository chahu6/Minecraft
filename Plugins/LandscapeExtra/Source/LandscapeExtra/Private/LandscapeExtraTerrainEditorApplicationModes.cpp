#include "LandscapeExtraTerrainEditorApplicationModes.h"

const FName FLandscapeExtraTerrainEditorApplicationModes::NoiseMode("NoiseMode");
const FName FLandscapeExtraTerrainEditorApplicationModes::ErosionMode("ErosionMode");

FText FLandscapeExtraTerrainEditorApplicationModes::GetLocalizedMode(const FName InMode)
{
	if (InMode == FLandscapeExtraTerrainEditorApplicationModes::NoiseMode)
	{
		return NSLOCTEXT("LandscapeModes", "NoiseMode", "Noise");
	}
	else if (InMode == FLandscapeExtraTerrainEditorApplicationModes::ErosionMode)
	{
		return NSLOCTEXT("LandscapeModes", "ErosionMode", "Erosion");
	}
	return FText::GetEmpty();
}
