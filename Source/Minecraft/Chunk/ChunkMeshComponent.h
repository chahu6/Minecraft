#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/MinecraftType/FaceType.h"
#include "ProceduralMeshComponent.h"
#include "Engine/DataTable.h"
#include "Minecraft/MinecraftType/MeshData.h"
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

class AChunk;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UChunkMeshComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChunkMeshComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Render();

	void BuildMesh();

	void ClearMeshData();

private:
	FBlockInfoTableRow* GetBlockInfo(uint8 BlockID);

private:
	UPROPERTY()
	class UProceduralMeshComponent* ProduralMesh;

private:
	AChunk* Chunk = nullptr;

	TMap<uint8, FMeshData> MeshDatas;
};
