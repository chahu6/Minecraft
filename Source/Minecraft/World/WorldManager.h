#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Minecraft/Generation/TerrainGenerator.h"
#include "WorldManager.generated.h"

class AChunk;

UCLASS()
class MINECRAFT_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Render();

	AChunk* GetChunk(int32 Index);

private:
	void BuildChunks();

	void BuildChunkMesh();

private:
	TArray<AChunk*> Chunks;

	TUniquePtr<ITerrainGenerator> TerrainGenerator;
};
