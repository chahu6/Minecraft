#include "MCPlayerController.h"
#include "Minecraft/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Minecraft/World/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/World/WorldManager.h"
#include "Minecraft/Chunk/ChunkSection.h"
#include "MinecraftPlayerCameraManager.h"

#include "SimplexNoiseLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Minecraft/Generation/Noise/NormalNoise.h"
#include "Minecraft/Math/Spline.h"

AMCPlayerController::AMCPlayerController()
{
	PlayerCameraManagerClass = AMinecraftPlayerCameraManager::StaticClass();
}

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

void AMCPlayerController::RemoveBlock()
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

UTexture2D* AMCPlayerController::CreateTextureFromArray()
{
	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	TSharedPtr<NormalNoise> Offset = NormalNoise::Create(-3, { 1.0, 1.0, 1.0, 0.0 });
	TSharedPtr<NormalNoise> Continentalness = NormalNoise::Create(-9, { 1.0, 1.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0 });
	TSharedPtr<NormalNoise> Weirdness = NormalNoise::Create(-7, { 1.0, 2.0, 1.0, 0.0, 0.0, 0.0 });
	TSharedPtr<NormalNoise> Erosion = NormalNoise::Create(-9, { 1.0, 1.0, 0.0, 1.0, 1.0 });
	TSharedPtr<NormalNoise> Humidity = NormalNoise::Create(-8, { 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 });
	TSharedPtr<NormalNoise> Temperature = NormalNoise::Create(-10, { 1.5, 0.0, 1.0, 0.0, 0.0, 0.0 });

	int32 TextureHeight = 1024;
	int32 TextureWidth = 1024;
	UTexture2D* TheTexture2D = UTexture2D::CreateTransient(TextureHeight, TextureWidth, PF_B8G8R8A8);
	uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
	for (int32 y = 0; y < TextureHeight; ++y)
	{
		for (int32 x = 0; x < TextureWidth; ++x)
		{
			int32 CurPixelIndex = ((y * TextureWidth) + x);

			double Color = 0;
			/*
			double ColorX = x + 4 * Offset->GetValue(x, 0, y);
			double ColorY = y + 4 * Offset->GetValue(y, x, 0);
			
			Color = Erosion->GetValue(ColorX, 0, ColorY);

			if (Color < -0.9)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), Color));
				*/

			Color /= (1 + 0.5 + 0.25);
			Color = FMath::Pow(Color * FudgeFactor, Exponent);
			Color = UKismetMathLibrary::MapRangeClamped(Color, -1, 1, 0, 255);

			Pixels[4 * CurPixelIndex] = Color;
			Pixels[4 * CurPixelIndex + 1] = Color;
			Pixels[4 * CurPixelIndex + 2] = Color;
			Pixels[4 * CurPixelIndex + 3] = 255;
		}
	}

	void* TextureData = TheTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureWidth * TextureHeight * 4);
	TheTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	TheTexture2D->UpdateResource();

	return TheTexture2D;
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
	const FVector& CameraLocation = PlayerCameraManager->ViewTarget.POV.Location;
	const FRotator& CameraRotation = PlayerCameraManager->ViewTarget.POV.Rotation;

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

void AMCPlayerController::Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z)
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

void AMCPlayerController::Rebuild_Adjacent_Chunks()
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
