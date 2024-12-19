#pragma once

#include "CoreMinimal.h"
#include "Components/ObjectPoolComponent.h"
#include "ChunkPoolComponent.generated.h"

struct FChunkPos;
class AChunk;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINECRAFT_API UChunkPoolComponent : public UObjectPoolComponent
{
	GENERATED_BODY()

public:
	UChunkPoolComponent();

	virtual void BeginPlay() override;

	AChunk* SpawnChunk(const FChunkPos& ChunkPos);
};