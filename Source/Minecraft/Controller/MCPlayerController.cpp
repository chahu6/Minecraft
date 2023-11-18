#include "MCPlayerController.h"
#include "Minecraft/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "Minecraft/Character/MCPlayer.h"
#include "Camera/CameraComponent.h"
#include "Minecraft/World/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/World/WorldManager.h"
#include "Minecraft/Chunk/Chunk.h"
#include "Minecraft/Utils/Utils.h"

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MinecraftHUD = Cast<AMinecraftHUD>(GetHUD());
}

void AMCPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RayCast();
}

void AMCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MCPlayer = Cast<AMCPlayer>(GetPawn());
}

void AMCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowDebugInfo", IE_Pressed, this, &AMCPlayerController::ShowDebugInfo);
}

void AMCPlayerController::AddBlock()
{
	if (BlockData.BlockID > 0)
	{
		FBlockData Temp;
		uint8 BlockID = GetBlockID(BlockData.VoxelWorldPosition + BlockData.Normal, Temp);

		if (BlockID == 0)
		{
			AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
			if (WorldManager)
			{
				WorldManager->GetChunk(Temp.ChunkIndex)->SetBlock(Temp.BlockIndex, 2);
				WorldManager->GetChunk(Temp.ChunkIndex)->Rebuild();
			}
		}
	}
}

void AMCPlayerController::RemoveBlock()
{
	if (BlockData.BlockID > 0)
	{
		AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
		if (WorldManager)
		{
			WorldManager->GetChunk(BlockData.ChunkIndex)->SetBlock(BlockData.BlockIndex, 0);
			WorldManager->GetChunk(BlockData.ChunkIndex)->Rebuild();
			Rebuild_Adjacent_Chunks();
		}
	}
}

void AMCPlayerController::ShowDebugInfo()
{
	if (bIsDebug)
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->RemoveDebugInfo();
			bIsDebug = false;
		}
	}
	else
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->AddDebugInfo();
			bIsDebug = true;
		}
	}
}

void AMCPlayerController::RayCast()
{
	UCameraComponent* Camera = MCPlayer->GetCamera();

	// 起点与终点
	FVector Ray_Start = Camera->GetComponentLocation();
	FVector Ray_End = Camera->GetComponentLocation() + Camera->GetComponentRotation().Vector() * MAX_RAY_DIST * BlockSize;

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
			else if(Step_Dir == 1)
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

uint8 AMCPlayerController::GetBlockID(const FVector& VoxelWorldPosition, FBlockData& OutBlockData)
{
	int32 ChunkIndex_X = VoxelWorldPosition.X / CHUNK_SIZE;
	int32 ChunkIndex_Y = VoxelWorldPosition.Y / CHUNK_SIZE;
	int32 ChunkIndex_Z = VoxelWorldPosition.Z / CHUNK_SIZE;

	if (0 <= ChunkIndex_X && ChunkIndex_X < WORLD_W && 0 <= ChunkIndex_Y && ChunkIndex_Y < WORLD_D && 0 <= ChunkIndex_Z && ChunkIndex_Z < WORLD_H)
	{
		int32 ChunkIndex = ChunkIndex_X + ChunkIndex_Y * WORLD_W + ChunkIndex_Z * WORLD_AREA;

		AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
		if (WorldManager)
		{
			AChunk* Chunk = WorldManager->GetChunk(ChunkIndex);

			int32 Local_X = VoxelWorldPosition.X - ChunkIndex_X * CHUNK_SIZE;
			int32 Local_Y = VoxelWorldPosition.Y - ChunkIndex_Y * CHUNK_SIZE;
			int32 Local_Z = VoxelWorldPosition.Z - ChunkIndex_Z * CHUNK_SIZE;

			int32 BlockIndex = Local_X + Local_Y * CHUNK_SIZE + Local_Z * CHUNK_AREA;

			OutBlockData.BlockID = Chunk->GetBlock(BlockIndex);
			OutBlockData.BlockIndex = BlockIndex;
			OutBlockData.ChunkIndex = ChunkIndex;
			OutBlockData.VoxelLocalPosition = FVector(Local_X, Local_Y, Local_Z);
			OutBlockData.VoxelWorldPosition = VoxelWorldPosition;

			return OutBlockData.BlockID;
		}
	}

	return 0;
}

void AMCPlayerController::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z)
{
	AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
	if (WorldManager)
	{
		if (Utils::OutOfBounds(Chunk_World_X, 0, WORLD_W) && Utils::OutOfBounds(Chunk_World_Y, 0, WORLD_D) && Utils::OutOfBounds(Chunk_World_Z, 0, WORLD_H))
		{
			int32 ChunkIndex = Chunk_World_X + Chunk_World_Y * WORLD_W + Chunk_World_Z * WORLD_AREA;
			WorldManager->GetChunk(ChunkIndex)->Rebuild();
		}
	}
}

void AMCPlayerController::Rebuild_Adjacent_Chunks()
{
	// 获取Voxel位置
	int32 Voxel_Local_X = BlockData.VoxelLocalPosition.X;
	int32 Voxel_Local_Y = BlockData.VoxelLocalPosition.Y;
	int32 Voxel_Local_Z = BlockData.VoxelLocalPosition.Z;

	// 获取Voxel所在Chunk位置
	int32 Chunk_World_X = BlockData.VoxelWorldPosition.X / CHUNK_SIZE;
	int32 Chunk_World_Y = BlockData.VoxelWorldPosition.Y / CHUNK_SIZE;
	int32 Chunk_World_Z = BlockData.VoxelWorldPosition.Z / CHUNK_SIZE;

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
