#pragma once

#include "CoreMinimal.h"
#include "../MCEntity.h"
#include "InputActionValue.h"
#include "Minecraft/Interfaces/ItemInterface.h"
#include "Minecraft/Item/ItemStack.h"
#include "MCPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSwitchMainHand, int32);

UCLASS()
class MINECRAFT_API AMCPlayer : public AMCEntity, public IItemInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AddBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RemoveBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchPerspectivesAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenBackpackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* WheelAction;

public:
	AMCPlayer();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool AddItem(const ADroppedItem* Item) override;

private:
	void SwitchPerspectives();

	void AddBlock();
	void RemoveBlock();
	void OpenBackpack();
	void SwitchingItem(const FInputActionValue& Value);

	FItemStack GetMainHandItem();

public:
	FOnSwitchMainHand OnSwitchMainHand;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstCamera;

	//UPROPERTY(VisibleAnywhere, Category = "Camera")
	//UCameraComponent* FreeCamera;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UInteractiveComponent* InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UBackpackComponent* BackpackComponent;

private:
	enum class EPerspective : uint8
	{
		First,
		Third,
		Free
	};

	EPerspective NextPerspective = EPerspective::Third;

	int32 MainHandIndex = 0;
};
