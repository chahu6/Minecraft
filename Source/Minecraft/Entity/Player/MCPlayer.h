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
	class UInputAction* UseItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RemoveBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchPerspectivesAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenBackpackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* WheelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropItemAction;

public:
	AMCPlayer();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool AddItem(const ADroppedItem* DroppedItem) override;
	void UpdateMainHandItem();

	FItemStack GetMainHandItem();

	void ConsumeItemStack();

private:
	void SwitchPerspectives();

	void UseItem();

	void OnClickAction();

	void OngoinAction();

	void OnResetAction();

	void OpenBackpack();
	void SwitchingItem(const FInputActionValue& Value);
	void DropItem();

	void Initial();
public:
	FOnSwitchMainHand OnSwitchMainHand;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstCamera;

	//UPROPERTY(VisibleAnywhere, Category = "Camera")
	//UCameraComponent* FreeCamera;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UInteractiveComponent* InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UBackpackComponent* BackpackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UCraftingComponent* CraftingComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	class UDataTable* ItemsDataTable;

	enum class EPerspective : uint8
	{
		First,
		Third,
		Free
	};

	EPerspective NextPerspective = EPerspective::Third;

	int32 MainHandIndex = 0;

public:
	FORCEINLINE const UBackpackComponent* GetBackpackComponent() const { return BackpackComponent; }
};
