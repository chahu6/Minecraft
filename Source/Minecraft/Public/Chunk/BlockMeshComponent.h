#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "BlockMeshComponent.generated.h"

class AChunk;
class AWorldManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UBlockMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	UBlockMeshComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void Render(const TMap<int32, TSharedPtr<struct FMeshData>>& NewMeshDatas);

private:
	/**
	* ÆÕÍ¨Íø¸ñ»¯
	*/
	//void BuildChunkMesh();

	//bool IsVoid(const FIntVector& BlockWorldVoxelLocation, AWorldManager* WorldManager);

private:
	UPROPERTY()
	TObjectPtr<AChunk> Chunk;
};
