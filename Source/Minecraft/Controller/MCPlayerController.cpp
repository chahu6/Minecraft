#include "MCPlayerController.h"
#include "Minecraft/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "Minecraft/Character/MCPlayer.h"
#include "Camera/CameraComponent.h"
#include "Minecraft/World/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/World/WorldManager.h"
#include "Minecraft/Chunk/Chunk.h"

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

#if 1
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
		if (GetBlockData(Current_Voxel))
		{
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
			}
			else
			{
				Current_Voxel.Z += dz;
				tMaxZ += tDeltaZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				Current_Voxel.Y += dy;
				tMaxY += tDeltaY;
			}
			else
			{
				Current_Voxel.Z += dz;
				tMaxZ += tDeltaZ;
			}
		}
		if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1) break;
	}

#else

	/* 
	* 过时算法
	* 有问题的算法, 不够准确
	* 将在下次删除
	*/
	int32 Step_Dir = -1;

	// 射线的前进方向
	double StepX = (Ray[0] >= 0) ? 1 : -1;
	double StepY = (Ray[1] >= 0) ? 1 : -1;
	double StepZ = (Ray[2] >= 0) ? 1 : -1;

	// 射线方向的下一个边界
	double Next_Voxel_Boundary_X = (Current_Voxel[0] + StepX) * BlockSize;
	double Next_Voxel_Boundary_Y = (Current_Voxel[1] + StepY) * BlockSize;
	double Next_Voxel_Boundary_Z = (Current_Voxel[2] + StepZ) * BlockSize;

	//tMaxX： 从起点开始，第一次跟下一个X方向的边界相交的时间(如果没有交点，时间设为无穷大)
	double tMaxX = (Ray[0] != 0) ? (Next_Voxel_Boundary_X - Ray_Start[0]) / Ray[0] : DBL_MAX;
	double tMaxY = (Ray[1] != 0) ? (Next_Voxel_Boundary_Y - Ray_Start[1]) / Ray[1] : DBL_MAX;
	double tMaxZ = (Ray[2] != 0) ? (Next_Voxel_Boundary_Z - Ray_Start[2]) / Ray[2] : DBL_MAX;

	double tDeltaX = (Ray[0] != 0) ? BlockSize / Ray[0] * StepX : DBL_MAX;
	double tDeltaY = (Ray[1] != 0) ? BlockSize / Ray[1] * StepY : DBL_MAX;
	double tDeltaZ = (Ray[2] != 0) ? BlockSize / Ray[2] * StepZ : DBL_MAX;

	if (GetBlockData(Current_Voxel))
	{
		return;
	}

	FVector Diff(0.0);
	bool Neg_Ray = false;
	if (Current_Voxel[0] != Last_Voxel[0] && Ray[0] < 0) { Diff[0]--; Neg_Ray = true; }
	if (Current_Voxel[1] != Last_Voxel[1] && Ray[1] < 0) { Diff[1]--; Neg_Ray = true; }
	if (Current_Voxel[2] != Last_Voxel[2] && Ray[2] < 0) { Diff[2]--; Neg_Ray = true; }

	// 调试相关
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), Ray_Start, Ray_End, FColor::Red, false, -1.0f, 0U, 5);
		DrawDebugLine(GetWorld(), Current_Voxel * BlockSize, Last_Voxel * BlockSize, FColor::Blue, false, -1.0f, 0U, 5);
	}

	if (Neg_Ray)
	{
		Current_Voxel += Diff;

		if (GetBlockData(Current_Voxel))
		{
			return;
		}
	}

	double tx = tMaxX, ty = tMaxY, tz = tMaxZ;
	while (Last_Voxel != Current_Voxel)
	{
		if (tx < ty)
		{
			if (tx < tz) 
			{ 
				Current_Voxel[0] += StepX; 
				tx += tDeltaX; 
				Step_Dir = 0;
			}
			else 
			{ 
				Current_Voxel[2] += StepZ; 
				tz += tDeltaZ; 
				Step_Dir = 2;
			}
		}
		else
		{
			if (ty < tz) 
			{ 
				Current_Voxel[1] += StepY; 
				ty += tDeltaY; 
				Step_Dir = 1;
			}
			else 
			{ 
				Current_Voxel[2] += StepZ; 
				tz += tDeltaZ; 
				Step_Dir = 2;
			}
		}

		if (bIsDebug)
		{
			DrawDebugLine(GetWorld(), Current_Voxel * BlockSize, Last_Voxel * BlockSize, FColor::Green, false, -1.0f, 0U, 5);
		}

		if (GetBlockData(Current_Voxel))
		{
			return;
		}
	}
#endif
}

bool AMCPlayerController::GetBlockData(const FVector& VoxelWorldPosition)
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

			BlockData.BlockID = Chunk->GetBlock(BlockIndex);
			BlockData.BlockIndex = BlockIndex;
			BlockData.ChunkIndex = ChunkIndex;

			if (BlockData.BlockID == 0)
			{
				return false;
			}

			return true;
		}
	}

	return false;
}
