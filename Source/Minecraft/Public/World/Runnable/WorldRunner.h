#pragma once

#include "CoreMinimal.h"
#include "World/Runnable/CommonRunnable.h"

/**
 * 
 */
class MINECRAFT_API FWorldRunner : public FCommonRunnable
{
public:
	FWorldRunner(const FString& InThreadName, AWorldManager* InWorldManager);

protected:
	virtual void DoThreadedWork() override;
};
