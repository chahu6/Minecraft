// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlockBehavior.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MINECRAFT_API UBlockBehavior : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInit() {};

	virtual void OnDestroy() {};

	virtual void OnBeforePlace() {};

	virtual void OnAfterPlace() {};

	virtual void OnInteract() 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Behavior")));
	};

	virtual void OnUpdate() {};
};
