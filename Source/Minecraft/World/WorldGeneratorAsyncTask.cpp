#include "WorldGeneratorAsyncTask.h"
#include "WorldManager.h"

FWorldGeneratorAsyncTask::FWorldGeneratorAsyncTask(AWorldManager* WorldManager)
{
	this->WorldManager = WorldManager;
}

void FWorldGeneratorAsyncTask::DoWork()
{
	//WorldManager->UpdateWorldAsync();
	double OldTime = FPlatformTime::Seconds();
	double Result = 0;
	for (double i = 0; i < 1000000000; ++i)
	{
		Result += FMath::Sqrt(i) / 1000000000;
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), Result);
	double End = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Warning, TEXT("%u Second: %fms end"), __LINE__, (End - OldTime) * 1000);

}
