#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCCharacter.generated.h"

UCLASS()
class MINECRAFT_API AMCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMCCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SwitchPerspectives();

	void AddBlock();
	void RemoveBlock();

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstCamera;

	//UPROPERTY(VisibleAnywhere, Category = "Camera")
	//UCameraComponent* FreeCamera;

private:
	enum class EPerspective : uint8
	{
		First,
		Third,
		Free
	};

	EPerspective NextPerspective = EPerspective::Third;
};
