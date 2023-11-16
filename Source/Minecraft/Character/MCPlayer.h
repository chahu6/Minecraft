#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MCPlayer.generated.h"

UCLASS()
class MINECRAFT_API AMCPlayer : public APawn
{
	GENERATED_BODY()

public:
	AMCPlayer();

protected:
	// Actor���̺���
	//virtual void PostActorCreated() override;
	//virtual void OnConstruction(const FTransform& Transform) override;
	//virtual void PreInitializeComponents() override;
	//virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* FloatingPawnMovement;

public:
	FORCEINLINE UCameraComponent* GetCamera() const noexcept { return FollowCamera; };
};
