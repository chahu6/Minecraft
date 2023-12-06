#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Minecraft/Interfaces/ItemInterface.h"
#include "MCCharacter.generated.h"

UCLASS()
class MINECRAFT_API AMCCharacter : public ACharacter, public IItemInterface
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AddBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RemoveBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchPerspectivesAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenBackpackAction;

public:
	AMCCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool AddItem(const AItem* Item) override;

public:

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void SwitchPerspectives();

	void AddBlock();
	void RemoveBlock();
	void OpenBackpack();

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstCamera;

	//UPROPERTY(VisibleAnywhere, Category = "Camera")
	//UCameraComponent* FreeCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UInteractiveComponent* InteractiveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* BackpackComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* HotbarComp;

private:
	enum class EPerspective : uint8
	{
		First,
		Third,
		Free
	};

	EPerspective NextPerspective = EPerspective::Third;

public:
	FORCEINLINE UInventoryComponent* GetHotbarComp() const { return HotbarComp; }
};
