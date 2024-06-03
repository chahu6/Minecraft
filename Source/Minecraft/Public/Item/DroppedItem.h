#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Info/ItemInfo.h"
#include "ItemStack.h"
#include "DroppedItem.generated.h"

UCLASS()
class MINECRAFT_API ADroppedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ADroppedItem();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetItemStack(const FItemStack& NewItemStack);

protected:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box;

protected:
	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FItemStack ItemStack;

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;

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
	AActor* Player;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float InterpSeepd = 8.0f;

public:
	FORCEINLINE const FItemStack& GetItemStack() const { return ItemStack; }
};
