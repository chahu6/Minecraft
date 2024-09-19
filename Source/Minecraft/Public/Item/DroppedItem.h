#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Data/ItemInfo.h"
#include "DroppedItem.generated.h"

class UBoxComponent;
class USoundCue;

UCLASS()
class MINECRAFT_API ADroppedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ADroppedItem();
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void ConstructionInit();

	void SetItemData();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Box;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float FloatSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float FloatAmplitude = 10.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float RotationSpeed = 45.0f;

private:
	UPROPERTY()
	bool bIsPickingUp = false;

	UPROPERTY()
	TObjectPtr<AActor> Player;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float InterpSeepd = 15.0f;

public:
	FORCEINLINE void SetItemHandle(const FDataTableRowHandle& NewItemHandle) { ItemHandle = NewItemHandle; }
};
