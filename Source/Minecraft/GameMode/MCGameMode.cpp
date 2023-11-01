// Fill out your copyright notice in the Description page of Project Settings.


#include "MCGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MineCraft/Chunk/Chunk.h"
#include "SimplexNoiseLibrary.h"

void AMCGameMode::StartPlay()
{
	Super::StartPlay();
	
	// 设置随机数种子
	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	AddChunk();
}


void AMCGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsUpdated = UpdateLocation();
	if (bIsUpdated)
	{
		AddChunk();
		RemoveChunk();
	}
}

bool AMCGameMode::UpdateLocation()
{
	FVector2D NewLocation2D = FVector2D(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
	if (!UKismetMathLibrary::EqualEqual_Vector2DVector2D(ChunkLocation * ChunkSize, NewLocation2D, 100.0f))
	{
		ChunkLocation = (NewLocation2D / ChunkSize);
		return true;
	}

	return false;
}

void AMCGameMode::AddChunk()
{
	float x = ChunkLocation.X;
	float y = ChunkLocation.Y;
	float range = RenderingRange / ChunkSize;

	int32 maxX = x + range;
	int32 maxY = y + range;
	for (int32 i = x - range; i <= maxX; ++i)
	{
		for (int32 j = y - range; j <= maxY; ++j)
		{
			FVector2D ActualLocation(i * ChunkSize, j * ChunkSize);
			if (AllChunks.Contains(ActualLocation)) continue;

			double distance = UKismetMathLibrary::Distance2D(ActualLocation, ChunkLocation * ChunkSize);
			if (distance <= RenderingRange)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					AChunk* chunk = World->SpawnActor<AChunk>({ ActualLocation.X, ActualLocation.Y, 0.0f}, FRotator::ZeroRotator);
					if (chunk)
					{
						AllChunks.Add(ActualLocation, chunk);
					}
				}
			}
		}
	}
}

void AMCGameMode::RemoveChunk()
{
	TArray<FVector2D> ElementsToRemove;

	for (auto& Elem : AllChunks)
	{
		FVector2D Location = ChunkLocation * ChunkSize;
		double distance = UKismetMathLibrary::Distance2D(Location, Elem.Key);
		if (distance > RenderingRange)
		{
			ElementsToRemove.Add(Elem.Key);
			Elem.Value->Destroy();
		}
	}

	for (const auto& Element : ElementsToRemove)
	{
		AllChunks.Remove(Element);
	}
}
