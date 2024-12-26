#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "BlockMeshComponent.generated.h"

class AChunk;
class AWorldManager;
struct FGameplayTag;
struct FMeshData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UBlockMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	UBlockMeshComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void Render(const TMap<FGameplayTag, TSharedPtr<FMeshData>>& NewMeshDatas);

private:
	/**
	* ÆÕÍ¨Íø¸ñ»¯
	*/
	//void BuildChunkMesh();

	//bool IsVoid(const FIntVector& BlockWorldVoxelLocation, AWorldManager* WorldManager);

private:
	UPROPERTY()
	TWeakObjectPtr<AChunk> Chunk;
};
