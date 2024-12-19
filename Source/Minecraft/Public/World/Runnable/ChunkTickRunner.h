// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Runnable/CommonRunnable.h"

/**
 * 
 */
class MINECRAFT_API FChunkTickRunner : public FCommonRunnable
{
public:
	FChunkTickRunner(const FString& ThreadName, AWorldManager* InWorldManager);

protected:
	virtual void DoThreadedWork() override;
};
