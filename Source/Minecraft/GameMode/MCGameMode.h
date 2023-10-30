// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MCGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void StartPlay() override;

private:
	bool UpdateLocation();
	void AddChunk();
	void RemoveChunk();

private:
	FVector2D ChunkLocation = FVector2D::ZeroVector; // 实际是下标

	UPROPERTY(EditAnywhere)
	float ChunkSize = 1600.0f;

	UPROPERTY(EditAnywhere)
	float RenderingRange = 4800.0f;

	UPROPERTY(EditAnywhere)
	TMap<FVector2D, class AChunk*> AllChunks;
};
