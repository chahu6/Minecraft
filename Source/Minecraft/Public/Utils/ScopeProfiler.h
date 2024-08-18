#pragma once

#include "CoreMinimal.h"

class ScopeProfiler
{
public:
	ScopeProfiler(const FString& InName)
		:Name(InName)
	{
		OldTime = FPlatformTime::Seconds();
	}

	~ScopeProfiler()
	{
		double End = FPlatformTime::Seconds();
		if ((End - OldTime) * 1000 > 5.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Second: %fms end"), *Name, (End - OldTime) * 1000);
		}
	}

private:
	double OldTime = 0.f;
	FString Name;
};