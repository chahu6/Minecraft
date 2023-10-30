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
	float BlockSize = 100.0f;
};
