// Fill out your copyright notice in the Description page of Project Settings.


#include "MCGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MineCraft/Chunk/Chunk.h"

void AMCGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	bool bIsUpdated = UpdateLocation();
	if (!bIsUpdated) return;

	AddChunk();
	RemoveChunk();
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

	int maxX = x + range;
	int maxY = y + range;
	for (int i = x - range; i <= maxX; ++i)
	{
		for (int j = y - range; j <= maxY; ++j)
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
	for (auto& elem : AllChunks)
	{
		FVector2D Location = ChunkLocation * ChunkSize;
		double distance = UKismetMathLibrary::Distance2D(Location, elem.Key);
		if (distance > RenderingRange)
		{
			elem.Value->Destroy();
			AllChunks.Remove(elem.Key);
		}
	}
}
