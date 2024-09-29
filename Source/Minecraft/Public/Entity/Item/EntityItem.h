// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemStack.h"
#include "EntityItem.generated.h"

class UBoxComponent;

UCLASS()
class MINECRAFT_API AEntityItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AEntityItem();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	AActor* GetNearestPlayer();

protected:
	UPROPERTY(EditAnywhere, Category = Properties)
	FItemStack ItemStack;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float FloatSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float FloatAmplitude = 10.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float RotationSpeed = 45.0f;

	UPROPERTY()
	bool bIsPickingUp = false;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float DelayBeforeCanPickup = 0.5f;

	float DelayBeforeCanPickupTimer = 0.f;

	UPROPERTY()
	TArray<AActor*> Players;

	UPROPERTY(EditAnywhere, Category = "DefaultProperties")
	float InterpSeepd = 15.0f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> AvatarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Box;

public:
	FORCEINLINE void SetItemStack(const FItemStack& InItemStack) { ItemStack = InItemStack; }
};
