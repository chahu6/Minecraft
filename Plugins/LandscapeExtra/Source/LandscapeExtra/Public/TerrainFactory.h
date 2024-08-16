#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "TerrainFactory.generated.h"
/**
 * 
 */
UCLASS()
class LANDSCAPEEXTRA_API UTerrainFactory : public UFactory
{
	GENERATED_BODY()
public:
	UTerrainFactory();

	virtual bool CanCreateNew() const override;
	virtual bool ShouldShowInNewMenu() const override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
