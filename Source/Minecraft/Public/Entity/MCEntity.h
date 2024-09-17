// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCEntity.generated.h"

UCLASS()
class MINECRAFT_API AMCEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCEntity();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
