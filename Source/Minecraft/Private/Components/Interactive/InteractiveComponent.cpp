#include "Components/Interactive/InteractiveComponent.h"
#include "Player/EntityPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/MinecraftAssetLibrary.h"
#include "World/WorldManager.h"
#include "World/WorldGenerator.h"
#include "Init/Blocks.h"
#include "Item/ItemStack.h"
#include "Item/Item.h"
#include "Item/ItemBlock.h"

#include "TileEntity/TileEntity.h"

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

	if (MarkerMesh)
	{
		Marker->SetStaticMesh(MarkerMesh);
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
		MarkerLocation *= WorldGenerator::BlockSize;
		MarkerLocation.X += WorldGenerator::BlockSize >> 1;
		MarkerLocation.Y += WorldGenerator::BlockSize >> 1;
		MarkerLocation.Z += WorldGenerator::BlockSize >> 1;

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
	BlockVoxelLocation.X = FMath::FloorToInt32(Location.X / WorldGenerator::BlockSize);
	BlockVoxelLocation.Y = FMath::FloorToInt32(Location.Y / WorldGenerator::BlockSize);
	BlockVoxelLocation.Z = FMath::FloorToInt32(Location.Z / WorldGenerator::BlockSize);

	GEngine->AddOnScreenDebugMessage(24, 5.f, FColor::Red, FString::Printf(TEXT("WorldLocation: %s"), *WorldLocation.ToString()));
	GEngine->AddOnScreenDebugMessage(25, 5.f, FColor::Green, FString::Printf(TEXT("WorldNormal: %s"), *WorldNormal.ToString()));
	GEngine->AddOnScreenDebugMessage(26, 5.f, FColor::Blue, FString::Printf(TEXT("BlockVoxelLocation: %s"), *BlockVoxelLocation.ToString()));
}

FBlockState UInteractiveComponent::GetBlockDataFromLocation(const FVector& WorldLocation, const FVector& WorldNormal)
{
	FIntVector BlockVoxelLocation;
	WorldLocToBlockVoxelLoc(WorldLocation, WorldNormal, BlockVoxelLocation);

	return GetBlockDataFromLocation(BlockVoxelLocation);
}

FBlockState UInteractiveComponent::GetBlockDataFromLocation(const FIntVector& BlockVoxelLocation)
{
	AWorldManager* WorldManager = AWorldManager::Get();
	if (WorldManager)
	{
		FBlockState BlockState = WorldManager->GetBlockState(BlockVoxelLocation);
		return BlockState;
	}
	return {};
}

void UInteractiveComponent::UseItem()
{
	if (BlockHitResult.bBlockingHit && Player)
	{
		FIntVector BlockVoxelLocation;
		WorldLocToBlockVoxelLoc(BlockHitResult.ImpactPoint, BlockHitResult.ImpactNormal, BlockVoxelLocation);

		if (BlockHitResult.GetActor()->IsA<ATileEntity>())
		{
			ATileEntity* TileEntity = Cast<ATileEntity>(BlockHitResult.GetActor());
			TileEntity->OnBlockActivated(AWorldManager::Get(), BlockVoxelLocation, Player);
		}
		else
		{
			FBlockState BlockState = GetBlockDataFromLocation(BlockVoxelLocation);
			UBlock* Block = BlockState.GetBlock();

			if (Block->OnBlockActivated(AWorldManager::Get(), BlockVoxelLocation, Player))
			{
				UE_LOG(LogTemp, Warning, TEXT("Right Clicked!"));
			}
			else
			{
				FItemStack MainHandItemStack = Player->GetMainHandItem();
				if (MainHandItemStack.IsEmpty()) return;

				if (MainHandItemStack.GetItem()->OnItemUse(Player, AWorldManager::Get(), BlockVoxelLocation, BlockHitResult.ImpactNormal))
				{

				}
			}
		}
	}
}

bool UInteractiveComponent::OnPlayerDestroyBlock(const FVector& WorldLocation, const FVector& WorldNormal)
{
	FIntVector BlockVoxelLocation;
	WorldLocToBlockVoxelLoc(WorldLocation, WorldNormal, BlockVoxelLocation);

	return OnPlayerDestroyBlock(BlockVoxelLocation);
}

// @TODO
bool UInteractiveComponent::OnPlayerDestroyBlock(const FIntVector& BlockVoxelLocation)
{
	FBlockState BlockState = GetBlockDataFromLocation(BlockVoxelLocation);
	if (BlockState.IsAir()) return false;

	return RemoveBlockFromWorld(BlockVoxelLocation);
}

bool UInteractiveComponent::RemoveBlockFromWorld(const FIntVector& BlockVoxelLocation)
{
	AWorldManager* WorldManager = AWorldManager::Get();
	if (WorldManager)
	{
		return WorldManager->DestroyBlock(BlockVoxelLocation, true);
	}

	return false;
}

bool UInteractiveComponent::ClickBlock()
{
	if (!bIsHittingBlock)
	{
		bIsHittingBlock = true;
		WorldLocToBlockVoxelLoc(BlockHitResult.ImpactPoint, BlockHitResult.ImpactNormal, LastHitBlockLocation);

		AWorldManager* WorldManager = AWorldManager::Get();
		if (WorldManager)
		{
			FBlockState BlockState = WorldManager->GetBlockState(LastHitBlockLocation);
			if (!BlockState.IsAir())
			{
				BlockState.GetBlock()->OnBlockClicked(WorldManager, LastHitBlockLocation, GetOwner<AEntityPlayer>());
			}
		}
		return true;
	}

	return false;
}

void UInteractiveComponent::OngoingClick()
{
	if (OnPlayerDamageBlock())
	{

	}
}

// @TODO
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
		FBlockState BlockState = GetBlockDataFromLocation(BlockVoxelLocation);
		if (BlockState.GetBlock() == UBlocks::Air)
		{
			bIsHittingBlock = false;
			return false;
		}
		else
		{
			CurBlockDamageMP += BlockState.GetPlayerRelativeBlockHardness();

			if (CurBlockDamageMP >= 1.0f)
			{
				bIsHittingBlock = false;
				OnPlayerDestroyBlock(BlockVoxelLocation);
				CurBlockDamageMP = 0.f;
				BlockHitDelay = 5.f;
			}

			UpdateDestroyProgress(CurBlockDamageMP);
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
		UpdateDestroyProgress(CurBlockDamageMP);
	}
}

bool UInteractiveComponent::RayCast()
{
	if (PlayerController == nullptr) return false;

	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

	FVector RayEnd = CameraLocation + CameraRotation.Vector() * WorldGenerator::MAX_RAY_DIST * WorldGenerator::BlockSize;

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
