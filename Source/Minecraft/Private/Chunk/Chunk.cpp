#include "Chunk/Chunk.h"
#include "Chunk/BlockMeshComponent.h"
#include "World/WorldManager.h"
#include "World/Data/BlockState.h"
#include "World/Data/ChunkData.h"
#include "World/Gen/TerrainBase.h"
#include "SaveGame/ChunkSaveGame.h"
#include "SaveGame/NBT/KNBTTagCompound.h"
#include "World/WorldGenerator.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/ArchiveSaveCompressedProxy.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BlockMeshComponent = CreateDefaultSubobject<UBlockMeshComponent>(TEXT("BlockMeshComponent"));
	BlockMeshComponent->SetupAttachment(RootComponent);
	BlockMeshComponent->bUseAsyncCooking = true;

	ThreadEvent = FPlatformProcess::GetSynchEventFromPool();
}

void AChunk::SetInUse(bool InUse)
{
	Super::SetInUse(InUse);

	if (InUse)
	{
		OnLoadChunk();
	}
	else
	{
		OnUnloadChunk();
	}
}

void AChunk::OnLoadChunk()
{
	WorldManager = Cast<AWorldManager>(GetOwner());

	SlotName = FString::Printf(TEXT("%ssaves/Gzip_uint16/%d_%d.dat"), *FPaths::ProjectSavedDir(), ChunkPos.X, ChunkPos.Y);
}

void AChunk::OnUnloadChunk()
{
	/*if (LoadType == EChunkLoadType::StrongLoaded)
	{
		SaveKNBT();
	}*/

	ChunkData = nullptr;
	ChunkPos = FChunkPos();
	ChunkState = EChunkState::None;
	LoadType = EChunkLoadType::NotLoad;
	WorldManager = nullptr;
	SlotName.Empty();

	BlockMeshComponent->ClearAllMeshSections();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AChunk::BeginPlay()
{
	Super::BeginPlay();

	WorldManager = Cast<AWorldManager>(GetOwner());
}

void AChunk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ThreadEvent)
	{
		FPlatformProcess::ReturnSynchEventToPool(ThreadEvent);
		ThreadEvent = nullptr;
	}
}

void AChunk::GenerateTerrain(UTerrainBase* InTerrainBase)
{
	if (IFileManager::Get().FileExists(*SlotName))
	{
		LoadChunkWithKNBTData();
	}
	else
	{
		InTerrainBase->GenerateTerrain(WorldManager.Get(), ChunkPos);
	}
}

void AChunk::GenerateBiome(UTerrainBase* InTerrainBase)
{
	if (!IFileManager::Get().FileExists(*SlotName) && LoadType == EChunkLoadType::StrongLoaded)
	{
		InTerrainBase->GenerateBiome(WorldManager.Get(), ChunkPos);
		//SaveKNBT();
	}
}

bool AChunk::SaveKNBT()
{
	bool bFlag = true;

	KNBTTagCompound* Knbt = new KNBTTagCompound(TEXT("Chunk"));

	Knbt->SetInt("Height", WorldGenerator::CHUNK_HEIGHT);
	Knbt->SetArray("HeightMap", ChunkData->HeightMap);
	Knbt->SetChunkBlockDataTensor("Blocks", ChunkData->BlockStateMap);
	Knbt->SetChunkBiome("Biomes", ChunkData->Biomes);

	FBufferArchive ToBinary;
	Knbt->Write(ToBinary);

	delete Knbt;
	Knbt = nullptr;

	if (ToBinary.Num() <= 0) return false;

	// 压缩数据
	TArray<uint8> CompressedData;
	FArchiveSaveCompressedProxy Compressor = FArchiveSaveCompressedProxy(CompressedData, NAME_Gzip);

	Compressor << ToBinary;
	Compressor.Flush();

	if (FFileHelper::SaveArrayToFile(CompressedData, *SlotName))
	{
		bFlag = true;
	}
	else
	{
		bFlag = false;
	}

	Compressor.FlushCache();
	CompressedData.Empty();

	ToBinary.FlushCache();
	ToBinary.Empty();

	ToBinary.Close();

	return bFlag;
}

bool AChunk::LoadChunkWithKNBTData()
{
	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *SlotName))
	{
		UE_LOG(LogTemp, Error, TEXT("--- FFILEHELPER:>> Invalid File"));
		return false;
	}
	
	FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(CompressedData, NAME_Gzip);

	if (Decompressor.GetError())
	{
		UE_LOG(LogTemp, Error, TEXT("--- FArchiveLoadCompressedProxy>> ERROR : File Was Not Compressed"));
		return false;
	}

	//Decompress
	FBufferArchive DecompressedBinaryArray;
	Decompressor << DecompressedBinaryArray;

	FMemoryReader Reader = FMemoryReader(DecompressedBinaryArray, true); // true, free data after done

	KNBTTagCompound* Knbt = new KNBTTagCompound(TEXT("Chunks"));
	Knbt->Read(Reader);

	SetChunkWithKNBTData(Knbt);

	delete Knbt;
	Knbt = nullptr;

	CompressedData.Empty();
	Decompressor.FlushCache();
	Reader.FlushCache();

	// Empty & Close Buffer 
	DecompressedBinaryArray.Empty();
	DecompressedBinaryArray.Close();

	return true;
}

bool AChunk::SetChunkWithKNBTData(KNBTTagCompound* Data)
{
	int32 ChunkHeight = Data->GetInt(TEXT("Height"));
	if (!Data->GetArray(TEXT("HeightMap"), ChunkData->HeightMap))
	{
		return false;
	}
	if (!Data->GetChunkBlockDataTensor("Blocks", ChunkData->BlockStateMap))
	{
		return false;
	}
	if (!Data->GetChunkBiome(TEXT("Biomes"), ChunkData->Biomes))
	{
		return false;
	}

	return true;
}

void AChunk::TickUpdate()
{
	
}

void AChunk::UpdateChunk()
{
	//SetChunkState(EChunkState::Rebuild);

	BuildMesh();
	//AWorldManager::Get()->TaskQueue.Enqueue(this);
}

//void AChunk::AddActiveVoxel(const FBlockData& BlockData)
//{
	/*if (!ActiveVoxels.Contains(BlockData))
	{
		ActiveVoxels.Add(BlockData);
	}*/
//}

void AChunk::RenderTerrainMesh()
{
	BlockMeshComponent->Render(WorldManager->WorldInfo.ChunkDataMap[ChunkPos]->MeshDataCache);
}

void AChunk::RenderTerrainMesh(const TMap<FGameplayTag, TSharedPtr<FMeshData>>& MeshDatas)
{
	BlockMeshComponent->Render(MeshDatas);
}

//void AChunk::SetBlockState(const FIntVector& BlockOffsetLocation, const FBlockState& BlockState)
//{
//	WorldManager->PlaceBlock(BlockWorldVoxelLocation, BlockState);
//	ChunkData->SetBlockState(BlockOffsetLocation, BlockState);
//}

void AChunk::Dirty()
{
	bIsDirty = true;
}

void AChunk::BuildMesh()
{
	// 方块Mesh
	//BlockMeshComponent->BuildMesh(GenerationMethod);

	// 植物Mesh
	//PlantMeshComponent->BuildMesh();

	bIsDirty = false;
}
