#include "Components/Interactive/InteractiveComponent.h"
#include "Entity/MinecraftPlayer.h"
#include "Item/DroppedItem.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/MinecraftAssetLibrary.h"
#include "World/WorldManager.h"
#include "World/Behavior/BlockBehavior.h"
#include "World/WorldSettings.h"

UInteractiveComponent::UInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// 辅助标记方块
	if (InitMarkComponent(GetOwner()->GetRootComponent()))
	{
		Marker->SetWorldRotation(FRotator::ZeroRotator);
		Marker->SetWorldScale3D(FVector(1.0001f));
		Marker->SetCastShadow(false);
		Marker->SetVisibility(false);
		if (MaterialInstance)
		{
			DestroyMaterial = Marker->CreateDynamicMaterialInstance(0, MaterialInstance);
			UpdateDestroyProgress(0.0f);
		}
	}

	PlayerController = Cast<APlayerController>(Player->Controller);
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

void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RayCast())
	{
		FIntVector MarkerLocation;
		WorldLocToBlockVoxelLoc(BlockHitResult.ImpactPoint, BlockHitResult.ImpactNormal, MarkerLocation);
		MarkerLocation *= WorldSettings::BlockSize;
		MarkerLocation.X += WorldSettings::BlockSize >> 1;
		MarkerLocation.Y += WorldSettings::BlockSize >> 1;
		MarkerLocation.Z += WorldSettings::BlockSize >> 1;

		Marker->SetVisibility(true);
		Marker->SetWorldLocation(FVector(MarkerLocation));
	}
	else
	{
		Marker->SetVisibility(false);
	}
}

void UInteractiveComponent::UpdateDestroyProgress(float Value)
{
	if (DestroyMaterial)
	{
		DestroyMaterial->SetScalarParameterValue(TEXT("Damage"), Value);
	}
}

void UInteractiveComponent::WorldLocToBlockVoxelLoc(const FVector& WorldLocation, const FVector& WorldNormal, FIntVector& BlockVoxelLocation)
{
	FVector Location = WorldLocation - WorldNormal;
	Location.X = FMath::FloorToInt32(Location.X);
	Location.Y = FMath::FloorToInt32(Location.Y);
	Location.Z = FMath::FloorToInt32(Location.Z);
	BlockVoxelLocation.X = FMath::FloorToInt32(Location.X / WorldSettings::BlockSize);
	BlockVoxelLocation.Y = FMath::FloorToInt32(Location.Y / WorldSettings::BlockSize);
	BlockVoxelLocation.Z = FMath::FloorToInt32(Location.Z / WorldSettings::BlockSize);
	GEngine->AddOnScreenDebugMessage(24, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *WorldLocation.ToString()));
	GEngine->AddOnScreenDebugMessage(25, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *WorldNormal.ToString()));
	GEngine->AddOnScreenDebugMessage(26, 5.f, FColor::Blue, FString::Printf(TEXT("%s"), *BlockVoxelLocation.ToString()));
}

FBlockData UInteractiveComponent::GetBlockDataFromLocation(const FVector& WorldLocation, const FVector& WorldNormal)
{
	FIntVector BlockVoxelLocation;
	WorldLocToBlockVoxelLoc(WorldLocation, WorldNormal, BlockVoxelLocation);

	return GetBlockDataFromLocation(BlockVoxelLocation);
}

FBlockData UInteractiveComponent::GetBlockDataFromLocation(const FIntVector& BlockVoxelLocation)
{
	AWorldManager* WorldManager = AWorldManager::Get();
	if (WorldManager)
	{
		FBlockData BlockData = WorldManager->GetBlock(BlockVoxelLocation);
		return BlockData;
	}
	return {};
}

void UInteractiveComponent::UseItem()
{
	if (BlockHitResult.bBlockingHit && Player)
	{
		FItemData MainHandItemData = Player->GetMainHandItem();
		if (!MainHandItemData.IsValid()) return;

		if (MainHandItemData.Type == EItemType::BuildingBlock)
		{
			PlaceBlock(MainHandItemData.ID);
		}
	}
}

void UInteractiveComponent::PlaceBlock(int32 ItemID)
{
	FIntVector BlockVoxelLocation;
	WorldLocToBlockVoxelLoc(BlockHitResult.ImpactPoint, BlockHitResult.ImpactNormal, BlockVoxelLocation);

	BlockVoxelLocation.X += BlockHitResult.ImpactNormal.X;
	BlockVoxelLocation.Y += BlockHitResult.ImpactNormal.Y;
	BlockVoxelLocation.Z += BlockHitResult.ImpactNormal.Z;

	FBlockData BlockData = GetBlockDataFromLocation(BlockVoxelLocation);

	if (BlockData.IsValid()) return;

	FBlockMeta BlockMeta;
	if (!UMinecraftAssetLibrary::GetBlockMeta(ItemID, BlockMeta)) return;

	if (BlockMeta.BehaviorClass) BlockMeta.BehaviorClass->GetDefaultObject<UBlockBehavior>()->OnInteract();

	AWorldManager* WorldManager = AWorldManager::Get();
	if (WorldManager)
	{
		if (BlockMeta.BehaviorClass) BlockMeta.BehaviorClass->GetDefaultObject<UBlockBehavior>()->OnBeforePlace();
		WorldManager->PlaceBlock(BlockVoxelLocation, BlockMeta.BlockID);

		FVector WorldLocation = FVector(BlockVoxelLocation * WorldSettings::BlockSize);
		WorldLocation = WorldLocation + (WorldSettings::BlockSize >> 1);
		if (BlockMeta.BehaviorClass) BlockMeta.BehaviorClass->GetDefaultObject<UBlockBehavior>()->OnAfterPlace(WorldManager, WorldLocation, BlockMeta.PlaceSound);

		Player->ConsumeItem();
		Player->UpdateMainHandItem();
	}
}

bool UInteractiveComponent::DestroyBlock(const FVector& WorldLocation, const FVector& WorldNormal)
{
	FIntVector BlockVoxelLocation;
	WorldLocToBlockVoxelLoc(WorldLocation, WorldNormal, BlockVoxelLocation);

	return DestroyBlock(BlockVoxelLocation);
}

bool UInteractiveComponent::DestroyBlock(const FIntVector& BlockVoxelLocation)
{
	FBlockData BlockData = GetBlockDataFromLocation(BlockVoxelLocation);
	if (!BlockData.IsValid()) return false;

	bool bIsDestroyed = RemoveBlockFromWorld(BlockVoxelLocation);
	if (bIsDestroyed)
	{
		FBlockMeta BlockMeta;
		bool bSuccessed = UMinecraftAssetLibrary::GetBlockMeta(BlockData.BlockID(), BlockMeta);
		if (bSuccessed)
		{
			checkf(DroppedItemClass, TEXT("Uninitialize DroppedItemClass"));

			FVector WorldLocation = FVector(BlockVoxelLocation * WorldSettings::BlockSize);
			WorldLocation.X += WorldSettings::BlockSize >> 1;
			WorldLocation.Y += WorldSettings::BlockSize >> 1;
			WorldLocation.Z += WorldSettings::BlockSize >> 1;

			ADroppedItem* DroppedItem = GetWorld()->SpawnActorDeferred<ADroppedItem>(DroppedItemClass, FTransform(FRotator::ZeroRotator, WorldLocation));
			DroppedItem->SetItemHandle(BlockMeta.ItemHandle);
			DroppedItem->FinishSpawning(DroppedItem->GetActorTransform());
			return true;
		}
	}

	return false;
}

bool UInteractiveComponent::RemoveBlockFromWorld(const FIntVector& BlockVoxelLocation)
{
	//AWorldManager* WorldManager = Cast<AWorldManager>(UGameplayStatics::GetActorOfClass(this, AWorldManager::StaticClass()));
	AWorldManager* WorldManager = AWorldManager::Get();
	if (WorldManager)
	{
		return WorldManager->DestroyBlock(BlockVoxelLocation);
	}

	return false;
}

bool UInteractiveComponent::ClickBlock()
{
	if (!bIsHittingBlock)
	{
		bIsHittingBlock = true;
		WorldLocToBlockVoxelLoc(BlockHitResult.ImpactPoint, BlockHitResult.ImpactNormal, LastHitBlockLocation);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("点击Block")));
		return true;
	}

	return false;
}

void UInteractiveComponent::OngoingClick()
{
	if (OnPlayerDamageBlock())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("播放挥手动画和特效"));
	}
}

bool UInteractiveComponent::OnPlayerDamageBlock()
{
	if (!BlockHitResult.bBlockingHit)
	{
		bIsHittingBlock = false;
		return false;
	}

	FIntVector BlockVoxelLocation;
	WorldLocToBlockVoxelLoc(BlockHitResult.ImpactPoint, BlockHitResult.ImpactNormal, BlockVoxelLocation);

	if (BlockHitDelay > 0)
	{
		--BlockHitDelay;
		return true;
	}
	else if (IsHittingPosition(BlockVoxelLocation))
	{
		FBlockData BlockData = GetBlockDataFromLocation(BlockVoxelLocation);

		if (!BlockData.IsValid())
		{
			bIsHittingBlock = false;
			return false;
		}
		else
		{
			FBlockMeta BlockMeta;
			bool bSuccessed = UMinecraftAssetLibrary::GetBlockMeta(BlockData.BlockID(), BlockMeta);
			if (!bSuccessed) return false;

			CurBlockDamageMP += BlockMeta.Hardness * GetWorld()->GetDeltaSeconds();

			DestroyPercent = CurBlockDamageMP / BlockMeta.Hardness;

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("播放音乐"));

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("更新破坏进度: %f"), DestroyPercent));
			DestroyMaterial->SetScalarParameterValue(TEXT("Damage"), DestroyPercent);

			if (DestroyPercent >= 1.0f)
			{
				DestroyBlock(BlockVoxelLocation);
				bIsHittingBlock = false;
				CurBlockDamageMP = 0.0f;
				BlockHitDelay = 5.0f;
				DestroyPercent = 0.0f;
				DestroyMaterial->SetScalarParameterValue(TEXT("Damage"), DestroyPercent);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("破坏方块"));
			}
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

void UInteractiveComponent::ResetBlockRemoving()
{
	if (bIsHittingBlock)
	{
		bIsHittingBlock = false;
		CurBlockDamageMP = 0.0f;
		BlockHitDelay = 0.0;
		DestroyPercent = 0.0f;
		UpdateDestroyProgress(CurBlockDamageMP);
	}
}

bool UInteractiveComponent::RayCast()
{
	if (PlayerController == nullptr) return false;

	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

	FVector RayEnd = CameraLocation + CameraRotation.Vector() * WorldSettings::MAX_RAY_DIST * WorldSettings::BlockSize;

	if (UWorld* World = GetWorld())
	{
		if(World->LineTraceSingleByChannel(BlockHitResult, CameraLocation, RayEnd, ECollisionChannel::ECC_Visibility))
		{

			return true;
		}
	}
	return false;
}

bool UInteractiveComponent::IsHittingPosition(const FIntVector& BlockVoxelLocation)
{
	return LastHitBlockLocation == BlockVoxelLocation;
}
