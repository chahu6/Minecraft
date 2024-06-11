#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitResult/BlockHitResult.h"
#include "InteractiveComponent.generated.h"

class AMinecraftPlayer;
class FBlock;

UENUM(BlueprintType)
enum class Action : uint8
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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UseItem();

	bool RemoveBlockFromWorld(const FBlockPos& BlockPos);

	void OngoingClick();
	bool OnPlayerDamageBlock(const FBlockHitResult& HitResult);
	bool ClickBlock();

	void ResetBlockRemoving();

	bool RayCast(FBlockHitResult& HitResult);

	bool IsHittingPosition(const FBlockHitResult& HitResult);

	//bool DestroyBlock(const FBlock* Block, const FBlockHitResult& HitResult);

	uint8 GetBlockID(const FVector& VoxelWorldPosition, FBlockHitResult& OutHitResult);

	void Rebuild_Adj_Chunk(int32 Chunk_World_X, int32 Chunk_World_Y, int32 Chunk_World_Z);

	void Rebuild_Adjacent_Chunks(const FBlockPos& BlockPos);

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

	UPROPERTY()
	UMaterialInstanceDynamic* DestroyMaterial;

	FBlockHitResult BlockHitResult;

	bool bIsDebug = false;
	bool bIsHittingBlock;
	int32 BlockHitDelay;
	float CurBlockDamageMP;

	FBlockPos CurrentBlock;

	FBlockHitResult CurrentHitResult;

public:
	[[nodiscard]] FORCEINLINE const FBlockHitResult& GetBlockHitResult() const { return BlockHitResult; }
};
