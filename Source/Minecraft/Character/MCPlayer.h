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
	// ActorÁ÷³Ìº¯Êý
	//virtual void PostActorCreated() override;
	//virtual void OnConstruction(const FTransform& Transform) override;
	//virtual void PreInitializeComponents() override;
	//virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SwitchPerspectives();

	void AddBlock();
	void RemoveBlock();

	void Jump();
	void StopJump();

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FirstCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FreeCamera;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	class UMinecraftPlayerMovement* MinecraftPlayerMovement;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

private:
	enum class EPerspective : uint8
	{
		First,
		Third,
		Free
	};

	EPerspective NextPerspective = EPerspective::Third;

	bool bIsJumping = false;

public:
	FORCEINLINE UCameraComponent* GetCamera() const noexcept { return FirstCamera; };
};
