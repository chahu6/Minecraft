#pragma once

#include "CoreMinimal.h"

class ScopeProfiler
{
public:
	ScopeProfiler()
	{
		OldTime = FPlatformTime::Seconds();
	}

	~ScopeProfiler()
	{
		double End = FPlatformTime::Seconds();
		//if ((End - OldTime) * 1000 > 1.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%u Second: %fms end"), __LINE__, (End - OldTime) * 1000);
		}
	}

private:
	double OldTime = 0.f;
};