#pragma once

#include "CoreMinimal.h"

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute() = 0;
};