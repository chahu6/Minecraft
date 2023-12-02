#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MinecraftPlayerMovement.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMinecraftPlayerMovement : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float MaxVelocity = 300.0f;

};
