// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlockBehavior.generated.h"

class AWorldManager;
class USoundBase;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MINECRAFT_API UBlockBehavior : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInit() {};

	virtual void OnInteract();

	virtual void OnBeforePlace();

	virtual void OnAfterPlace(AWorldManager* WorldManager, const FVector& Location, USoundBase* Sound);

	virtual void OnDestroy(AWorldManager* WorldManager, const FVector& Location, USoundBase* Sound);

	virtual void OnUpdate() {};
};
