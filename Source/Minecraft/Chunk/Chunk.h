#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

UCLASS()
class MINECRAFT_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunk();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void CreateBlock(float rangeMin, float rangeMax, float inFactor = 0.0001f);

private:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* Block;

	float BlockSize = 100.0f;
};
