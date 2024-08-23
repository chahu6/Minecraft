#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/Block/Block.h"
#include "InteractiveComponent.generated.h"

class AMinecraftPlayer;
class ADroppedItem;

UENUM(BlueprintType)
enum class EAction : uint8
{
	None = 0,

	START_DESTROY_BLOCK,
	ABORT_DESTROY_BLOCK,
	STOP_DESTROY_BLOCK,
	DROP_ALL_ITEMS,
	DROP_ITEM,
	RELEASE_USE_ITEM,
	SWAP_ITEM_WITH_OFFHAND
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class MINECRAFT_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AMinecraftPlayer;
public:	
	UInteractiveComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	bool DestroyBlock(const FVector& WorldLocation, const FVector& WorldNormal);
	bool DestroyBlock(const FIntVector& BlockVoxelLocation);

	FBlockData GetBlockDataFromLocation(const FVector& WorldLocation, const FVector& WorldNormal);
	FBlockData GetBlockDataFromLocation(const FIntVector& BlockVoxelLocation);

	void WorldLocToBlockVoxelLoc(const FVector& WorldLocation, const FVector& WorldNormal, FIntVector& BlockVoxelLocation);

	void UseItem();

	void PlaceBlock(int32 ItemID);

	bool RemoveBlockFromWorld(const FIntVector& BlockVoxelLocation);

	void OngoingClick();
	bool OnPlayerDamageBlock();
	bool ClickBlock();

	void ResetBlockRemoving();

	bool RayCast();

	bool IsHittingPosition(const FVector& WorldLocation);

	bool InitMarkComponent(USceneComponent* Parent);

	void UpdateDestroyProgress(float Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Marker;

private:
	UPROPERTY()
	TObjectPtr<AMinecraftPlayer> Player;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADroppedItem> DroppedItemClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> MaterialInstance;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DestroyMaterial;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bIsDebug = false;

private:
	bool bIsHittingBlock = false;
	int32 BlockHitDelay = 0;
	float CurBlockDamageMP = 0.0f;
	float DestroyPercent = 0.0f;

	FVector LastHitLocation;

	FHitResult BlockHitResult;
};
