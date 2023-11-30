#pragma once

#include "CoreMinimal.h"

class AWorldManager;
/**
 * ∑œ”√¡À
 */
class MINECRAFT_API FWorldRunner : public FRunnable
{
public:
	FWorldRunner(AWorldManager* WorldManager);
	~FWorldRunner();

public:
	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

private:
	FRunnableThread* WorldRunnerThread;

	AWorldManager* WorldManager;
};
