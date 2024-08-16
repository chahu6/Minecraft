#include "TerrainFactory.h"
#include "LandscapeExtraTerrain.h"

UTerrainFactory::UTerrainFactory()
{
	SupportedClass = ULandscapeExtraTerrain::StaticClass();
	bCreateNew = true;
}

bool UTerrainFactory::CanCreateNew() const
{
	return true;
}

bool UTerrainFactory::ShouldShowInNewMenu() const
{
	return true;
}

UObject* UTerrainFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<ULandscapeExtraTerrain>(InParent, InClass, InName, Flags);
}
