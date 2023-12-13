#include "InteractiveComponent.h"
#include "Minecraft/World/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Minecraft/World/WorldManager.h"
#include "Minecraft/Chunk/ChunkSection.h"
#include "Minecraft/Block/Blocks.h"
#include "Minecraft/Entity/Player/MCPlayer.h"

#include "Minecraft/Item/Items.h"

UInteractiveComponent::UInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (InitMarkComponent(GetOwner()->GetRootComponent()))
	{
		Marker->SetWorldRotation(FRotator::ZeroRotator);
		Marker->SetWorldScale3D(FVector(1.01f));
		Marker->SetCastShadow(false);
		Marker->SetVisibility(false);
		UMaterialInstance* Instance = LoadObject<UMaterialInstance>(this, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Minecraft/Assets/Materials/Minecraft/MI_Mark.MI_Mark'"));
		if (Instance)
		{
			DestroyMaterial = Marker->CreateDynamicMaterialInstance(0, Instance);
			UpdateDestroyProgress(0.0f);
		}
	}

	PlayerController = Cast<APlayerController>(Player->Controller);
}

void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RayCast(BlockHitResult))
	{
		Marker->SetVisibility(true);
		Marker->SetWorldLocation(BlockHitResult.BlockPos.WorldLocation() + BlockSize / 2.0f);
	}
	else
	{
		Marker->SetVisibility(false);
	}
}

bool UInteractiveComponent::InitMarkComponent(USceneComponent* Parent)
{
	Marker = NewObject<UStaticMeshComponent>(this, TEXT("MarkerComponent"));
	Marker->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	Marker->RegisterComponent();

	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(this, TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeMesh)
	{
		Marker->SetStaticMesh(CubeMesh);
	}
	Marker->SetCollisionProfileName(TEXT("NoCollision"));
	return Marker->IsRegistered();
}

void UInteractiveComponent::UpdateDestroyProgress(float Value)
{
	DestroyMaterial->SetScalarParameterValue(TEXT("Damage"), Value);
}

void UInteractiveComponent::AddBlock()
{
	if (BlockHitResult.BlockID > 0)
	{
		FBlockHitResult Temp;
		uint8 BlockID = GetBlockID(BlockHitResult.BlockPos.VoxelWorldLocation() + BlockHitResult.Direction, Temp);

		if (BlockID == 0)
		{
			AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
			if (WorldManager)
			{
				AChunkSection* ChunkSection = WorldManager->GetChunkSection(Temp.BlockPos);
				if (ChunkSection)
				{
					ChunkSection->SetBlock(Temp.BlockPos.OffsetLocation(), 2);
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

bool UInteractiveComponent::RemoveBlockFromWorld(const FBlockPos& BlockPos)
{
	AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
	if (WorldManager)
	{
		AChunkSection* ChunkSection = WorldManager->GetChunkSection(BlockPos);
		ChunkSection->SetBlock(BlockPos.OffsetLocation(), 0);

		// 重新计算空值
		ChunkSection->RecalculateEmpty();
		ChunkSection->Rebuild();
		Rebuild_Adjacent_Chunks(BlockPos);

		return true;
	}

	return false;
}

bool UInteractiveComponent::ClickBlock()
{
	if (!bIsHittingBlock)
	{
		bIsHittingBlock = true;
		CurrentHitResult = GetBlockHitResult();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("点击Block")));
		return true;
	}

	return false;
}

void UInteractiveComponent::ResetBlockRemoving()
{
	if (bIsHittingBlock)
	{
		bIsHittingBlock = false;
		CurBlockDamageMP = 0.0f;
		BlockHitDelay = 0.0;
		UpdateDestroyProgress(CurBlockDamageMP);
	}
}

void UInteractiveComponent::OngoingClick()
{
	if (OnPlayerDamageBlock(GetBlockHitResult()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("播放挥手动画和特效"));
	}
}

bool UInteractiveComponent::OnPlayerDamageBlock(const FBlockHitResult& HitResult)
{
	if (!HitResult.bIsHit) return false;

	if (BlockHitDelay > 0)
	{
		--BlockHitDelay;
		return true;
	}
	//else if()// 创建模式以后再说
	else if (IsHittingPosition(HitResult))
	{
		FBlock* HitBlock = FBlock::GetBlock(HitResult.BlockID).Get();

		ensure(HitBlock != nullptr);

		if (HitBlock->IsAir())
		{
			check(false);

			bIsHittingBlock = false;
			return false;
		}
		else
		{
			CurBlockDamageMP += HitBlock->GetPlayerRelativeBlockHardness(Player) * GetWorld()->GetDeltaSeconds();

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("播放音乐"));

			if (CurBlockDamageMP >= 1.0f)
			{
				DestroyBlock(HitBlock, HitResult);
				bIsHittingBlock = false;
				CurBlockDamageMP = 0.0f;
				BlockHitDelay = 5.0f;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("破坏方块"));
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("更新破坏进度"));
			DestroyMaterial->SetScalarParameterValue(TEXT("Damage"), CurBlockDamageMP);
			return true;
		}
	}
	else
	{
		ResetBlockRemoving();
		return ClickBlock();
	}

	return false;
}

bool UInteractiveComponent::RayCast(FBlockHitResult& HitResult)
{
	if (PlayerController == nullptr) return false;

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
	HitResult.Direction = FVector::ZeroVector;

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
		if (GetBlockID(Current_Voxel, HitResult) > 0)
		{
			HitResult.bIsHit = true;
			if (Step_Dir == 0)
			{
				HitResult.Direction.X = -dx;
			}
			else if (Step_Dir == 1)
			{
				HitResult.Direction.Y = -dy;
			}
			else if (Step_Dir == 2)
			{
				HitResult.Direction.Z = -dz;
			}
			return true;
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

	HitResult.bIsHit = false;
	return false;
}

bool UInteractiveComponent::IsHittingPosition(const FBlockHitResult& HitResult)
{
	return CurrentHitResult == HitResult;
}

bool UInteractiveComponent::DestroyBlock(const FBlock* Block, const FBlockHitResult& HitResult)
{
	bool bIsDestroyed = RemoveBlockFromWorld(HitResult.BlockPos);

	if (bIsDestroyed)
	{
		Block->DropBlockAsItem(GetWorld(), HitResult.BlockPos, HitResult.BlockID);
		Block->Destroyed();
		return true;
	}

	return false;
}

uint8 UInteractiveComponent::GetBlockID(const FVector& VoxelWorldPosition, FBlockHitResult& OutHitResult)
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

		OutHitResult.BlockID = ChunkSection->GetBlock(BlockIndex);
		OutHitResult.BlockPos.SetOffsetLocation(Local_X, Local_Y, Local_Z);
		OutHitResult.BlockPos.SetVoxelWorldLocation(VoxelWorldPosition.X, VoxelWorldPosition.Y, VoxelWorldPosition.Z);

		return OutHitResult.BlockID;
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

void UInteractiveComponent::Rebuild_Adjacent_Chunks(const FBlockPos& BlockPos)
{
	// 获取Voxel位置
	int32 Voxel_Local_X = BlockPos.X_OFFSET;
	int32 Voxel_Local_Y = BlockPos.Y_OFFSET;
	int32 Voxel_Local_Z = BlockPos.Z_OFFSET;

	// 获取Chunk所在Voxel位置
	int32 Chunk_World_X = FMath::Floor(BlockPos.X_VOXEL_WORLD / CHUNK_SIZE);
	int32 Chunk_World_Y = FMath::Floor(BlockPos.Y_VOXEL_WORLD / CHUNK_SIZE);
	int32 Chunk_World_Z = FMath::Floor(BlockPos.Z_VOXEL_WORLD / CHUNK_SIZE);

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