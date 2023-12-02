#include "InteractiveComponent.h"
#include "Minecraft/World/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/World/WorldManager.h"
#include "Minecraft/Chunk/ChunkSection.h"

UInteractiveComponent::UInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Character->Controller);
}

void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RayCast();
}

void UInteractiveComponent::AddBlock()
{
	if (BlockData.BlockID > 0)
	{
		uint8 BlockID = GetBlockID(BlockData.VoxelWorldPosition + BlockData.Normal, Temp);

		if (BlockID == 0)
		{
			AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
			if (WorldManager)
			{
				AChunkSection* ChunkSection = WorldManager->GetChunkSection(Temp.ChunkVoexlWorldPosition);
				if (ChunkSection)
				{
					ChunkSection->SetBlock(Temp.BlockIndex, 2);
					if (ChunkSection->IsEmpty())
					{
						ChunkSection->SetEmpty(false);
					}
					ChunkSection->Rebuild();
				}
			}
		}
	}
}

void UInteractiveComponent::RemoveBlock()
{
	if (BlockData.BlockID > 0)
	{
		AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
		if (WorldManager)
		{
			AChunkSection* ChunkSection = WorldManager->GetChunkSection(BlockData.ChunkVoexlWorldPosition);
			ChunkSection->SetBlock(BlockData.BlockIndex, 0);

			// 重新计算空值
			ChunkSection->RecalculateEmpty();
			ChunkSection->Rebuild();
			Rebuild_Adjacent_Chunks();
		}
	}
}

void UInteractiveComponent::RayCast()
{
	if (PlayerController == nullptr) return;

	const FVector& CameraLocation = PlayerController->PlayerCameraManager->ViewTarget.POV.Location;
	const FRotator& CameraRotation = PlayerController->PlayerCameraManager->ViewTarget.POV.Rotation;

	// 起点与终点
	FVector Ray_Start = CameraLocation;
	FVector Ray_End = CameraLocation + CameraRotation.Vector() * MAX_RAY_DIST * BlockSize;

	// 起始体素位置
	FVector Current_Voxel(FMath::Floor(Ray_Start.X / BlockSize),
						  FMath::Floor(Ray_Start.Y / BlockSize),
						  FMath::Floor(Ray_Start.Z / BlockSize));

	// 终点体素位置
	FVector Last_Voxel(FMath::Floor(Ray_End.X / BlockSize),
					   FMath::Floor(Ray_End.Y / BlockSize),
					   FMath::Floor(Ray_End.Z / BlockSize));

	// 射线方向向量
	FVector Ray = Ray_End - Ray_Start;

	// 射线方向标识符: 0 X, 1 Y, 2 Z
	int32 Step_Dir = -1;

	// 清除历史记录
	BlockData.Normal = FVector::ZeroVector;

	float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;

	int32 dx = FMath::Sign(Ray.X);
	if (dx != 0) tDeltaX = FMath::Min(BlockSize / (Ray.X) * dx, 10000000.0f); else tDeltaX = 10000000.0f;
	if (dx > 0) tMaxX = tDeltaX * (1 - FMath::Frac(Ray_Start.X / BlockSize)); else tMaxX = tDeltaX * FMath::Frac(Ray_Start.X / BlockSize);

	int32 dy = FMath::Sign(Ray.Y);
	if (dy != 0) tDeltaY = FMath::Min(BlockSize / (Ray_End.Y - Ray_Start.Y) * dy, 10000000.0f); else tDeltaY = 10000000.0f;
	if (dy > 0) tMaxY = tDeltaY * (1 - FMath::Frac(Ray_Start.Y / BlockSize)); else tMaxY = tDeltaY * FMath::Frac(Ray_Start.Y / BlockSize);

	int32 dz = FMath::Sign(Ray.Z);
	if (dz != 0) tDeltaZ = FMath::Min(BlockSize / (Ray_End.Z - Ray_Start.Z) * dz, 10000000.0f); else tDeltaZ = 10000000.0f;
	if (dz > 0) tMaxZ = tDeltaZ * (1 - FMath::Frac(Ray_Start.Z / BlockSize)); else tMaxZ = tDeltaZ * FMath::Frac(Ray_Start.Z / BlockSize);

	// 调试相关
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), Ray_Start, Ray_End, FColor::Red, false, -1.0f, 0U, 5);
		DrawDebugLine(GetWorld(), Current_Voxel * BlockSize, Last_Voxel * BlockSize, FColor::Blue, false, -1.0f, 0U, 5);
	}

	while (true)
	{
		if (GetBlockID(Current_Voxel, BlockData) > 0)
		{
			if (Step_Dir == 0)
			{
				BlockData.Normal.X = -dx;
			}
			else if (Step_Dir == 1)
			{
				BlockData.Normal.Y = -dy;
			}
			else if (Step_Dir == 2)
			{
				BlockData.Normal.Z = -dz;
			}
			return;
		}

		if (bIsDebug)
		{
			DrawDebugLine(GetWorld(), Current_Voxel * BlockSize, Last_Voxel * BlockSize, FColor::Green, false, -1.0f, 0U, 5);
		}

		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				Current_Voxel.X += dx;
				tMaxX += tDeltaX;
				Step_Dir = 0;
			}
			else
			{
				Current_Voxel.Z += dz;
				tMaxZ += tDeltaZ;
				Step_Dir = 2;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				Current_Voxel.Y += dy;
				tMaxY += tDeltaY;
				Step_Dir = 1;
			}
			else
			{
				Current_Voxel.Z += dz;
				tMaxZ += tDeltaZ;
				Step_Dir = 2;
			}
		}
		if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1) break;
	}
}

uint8 UInteractiveComponent::GetBlockID(const FVector& VoxelWorldPosition, FBlockData& OutBlockData)
{
	int32 ChunkWorld_X = FMath::Floor(VoxelWorldPosition.X / CHUNK_SIZE);
	int32 ChunkWorld_Y = FMath::Floor(VoxelWorldPosition.Y / CHUNK_SIZE);
	int32 ChunkWorld_Z = FMath::Floor(VoxelWorldPosition.Z / CHUNK_SIZE);

	FVector ChunkVoexlWorldPosition(ChunkWorld_X, ChunkWorld_Y, ChunkWorld_Z);

	AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
	if (WorldManager)
	{
		AChunkSection* ChunkSection = WorldManager->GetChunkSection(ChunkVoexlWorldPosition);

		// 在游玩时，因为地形一直是随着玩家的位置加载的所以完整游戏中，不因该为空
		if (ChunkSection == nullptr) return 0;

		int32 Local_X = VoxelWorldPosition.X - ChunkWorld_X * CHUNK_SIZE;
		int32 Local_Y = VoxelWorldPosition.Y - ChunkWorld_Y * CHUNK_SIZE;
		int32 Local_Z = VoxelWorldPosition.Z - ChunkWorld_Z * CHUNK_SIZE;

		int32 BlockIndex = Local_X + Local_Y * CHUNK_SIZE + Local_Z * CHUNK_AREA;

		OutBlockData.BlockID = ChunkSection->GetBlock(BlockIndex);
		OutBlockData.BlockIndex = BlockIndex;
		OutBlockData.VoxelLocalPosition = FVector(Local_X, Local_Y, Local_Z);
		OutBlockData.VoxelWorldPosition = VoxelWorldPosition;
		OutBlockData.ChunkVoexlWorldPosition = ChunkVoexlWorldPosition;

		return OutBlockData.BlockID;
	}

	return 0;
}

void UInteractiveComponent::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z)
{
	AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
	if (WorldManager)
	{
		AChunkSection* ChunkSection = WorldManager->GetChunkSection(FVector(Chunk_World_X, Chunk_World_Y, Chunk_World_Z));

		if (ChunkSection == nullptr)
			return;

		ChunkSection->Rebuild();
	}
}

void UInteractiveComponent::Rebuild_Adjacent_Chunks()
{
	// 获取Voxel位置
	int32 Voxel_Local_X = BlockData.VoxelLocalPosition.X;
	int32 Voxel_Local_Y = BlockData.VoxelLocalPosition.Y;
	int32 Voxel_Local_Z = BlockData.VoxelLocalPosition.Z;

	// 获取Chunk所在Voxel位置
	int32 Chunk_World_X = FMath::Floor(BlockData.VoxelWorldPosition.X / CHUNK_SIZE);
	int32 Chunk_World_Y = FMath::Floor(BlockData.VoxelWorldPosition.Y / CHUNK_SIZE);
	int32 Chunk_World_Z = FMath::Floor(BlockData.VoxelWorldPosition.Z / CHUNK_SIZE);

	// X轴
	if (Voxel_Local_X == 0)
	{
		Rebuild_Adj_Chunk(Chunk_World_X - 1, Chunk_World_Y, Chunk_World_Z);
	}
	else if (Voxel_Local_X == CHUNK_SIZE - 1)
	{
		Rebuild_Adj_Chunk(Chunk_World_X + 1, Chunk_World_Y, Chunk_World_Z);
	}

	// Y轴
	if (Voxel_Local_Y == 0)
	{
		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y - 1, Chunk_World_Z);
	}
	else if (Voxel_Local_Y == CHUNK_SIZE - 1)
	{
		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y + 1, Chunk_World_Z);
	}

	// Z轴
	if (Voxel_Local_Z == 0)
	{
		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y, Chunk_World_Z - 1);
	}
	else if (Voxel_Local_Z == CHUNK_SIZE - 1)
	{
		Rebuild_Adj_Chunk(Chunk_World_X, Chunk_World_Y, Chunk_World_Z + 1);
	}
}
