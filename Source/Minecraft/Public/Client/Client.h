// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Client.generated.h"

UCLASS()
class MINECRAFT_API AClient : public AActor
{
	GENERATED_BODY()
	
public:	
	AClient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
