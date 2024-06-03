#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Engine/DataTable.h"
#include "Types/MeshData.h"
#include "ChunkMeshComponent.generated.h"

USTRUCT(BlueprintType)
struct FBlockInfoTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UMaterialInterface* Material;
};

class AChunkSection;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UChunkMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	UChunkMeshComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Render();

	void BuildMesh();

	void ClearMeshData();

private:
	FBlockInfoTableRow* GetBlockInfo(uint8 BlockID);

private:
	UPROPERTY()
	AChunkSection* ChunkSection = nullptr;

	TMap<uint8, FMeshData> MeshDatas;
};
