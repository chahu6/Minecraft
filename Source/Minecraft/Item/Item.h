#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Info/ItemInfo.h"
#include "Item.generated.h"

UCLASS()
class MINECRAFT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetItemDetails(const FItemDetails& NewItemDetails);
	FItemDetails GetItemDetails() const;

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
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	int32 Count = 0;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float FloatSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float FloatAmplitude = 10.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float RotationSpeed = 45.0f;
};
