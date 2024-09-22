#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"

class AEntityPlayer;
class ADroppedItem;
struct FBlockState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class MINECRAFT_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AEntityPlayer;
public:	
	UInteractiveComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	bool OnPlayerDestroyBlock(const FVector& WorldLocation, const FVector& WorldNormal);
	bool OnPlayerDestroyBlock(const FIntVector& BlockVoxelLocation);

	FBlockState GetBlockDataFromLocation(const FVector& WorldLocation, const FVector& WorldNormal);
	FBlockState GetBlockDataFromLocation(const FIntVector& BlockVoxelLocation);

	void WorldLocToBlockVoxelLoc(const FVector& WorldLocation, const FVector& WorldNormal, FIntVector& BlockVoxelLocation);

	void UseItem();

	void PlaceBlock(int32 ItemID);

	bool RemoveBlockFromWorld(const FIntVector& BlockVoxelLocation);

	void OngoingClick();
	bool OnPlayerDamageBlock();
	bool ClickBlock();

	void ResetBlockRemoving();

	bool RayCast();

	bool IsHittingPosition(const FIntVector& BlockVoxelLocation);

	bool InitMarkComponent(USceneComponent* Parent);

	void UpdateDestroyProgress(float Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Marker;

private:
	UPROPERTY()
	TObjectPtr<AEntityPlayer> Player;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, Category = Initialization)
	TSubclassOf<ADroppedItem> DroppedItemClass;

	UPROPERTY(EditAnywhere, Category = Initialization)
	TObjectPtr<UStaticMesh> MarkerMesh;

	UPROPERTY(EditAnywhere, Category = Initialization)
	TObjectPtr<UMaterialInstance> MaterialInstance;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DestroyMaterial;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bIsDebug = false;

private:
	bool bIsHittingBlock = false;
	int32 BlockHitDelay = 0;
	float CurBlockDamageMP = 0.0f;

	FIntVector LastHitBlockLocation;

	FHitResult BlockHitResult;
};
