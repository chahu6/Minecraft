// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class OBJECTPOOL_API APooledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APooledActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SetInUse"))
	void ReceiveSetInUse(bool InUse);

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetInUse(bool InUse);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	float TimeToLive = 5.f;

	FTimerHandle TimeToLiveHandle;

public:
	FORCEINLINE bool InUse() const { return bInUse; }
	FORCEINLINE void SetTimeToLive(float NewTimeToLive) { TimeToLive = NewTimeToLive; }
};
