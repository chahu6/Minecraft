#pragma once

#include "CoreMinimal.h"
#include "MinecraftEntity.h"
#include "InputActionValue.h"
#include "Interfaces/InteractiveInterface.h"
#include "MinecraftPlayer.generated.h"

class UBackpackComponent;
class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UInteractiveComponent;
class UCraftingComponent;
class UInputAction;
class ADroppedItem;
struct FItemData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSwitchMainHand, int32);

UCLASS()
class MINECRAFT_API AMinecraftPlayer : public AMinecraftEntity, public IInteractiveInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UseItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RemoveBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchPerspectivesAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OpenBackpackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WheelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DropItemAction;

public:
	AMinecraftPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void UpdateMainHandItem();

	FItemData GetMainHandItem();

	void ConsumeItem();

	/** Interactive Interface */
	virtual bool AddItemToInventory_Implementation(ADroppedItem* DroppedItem) override;
	/** Interactive Interface end*/

private:
	void SwitchPerspectives();

	void UseItem();

	void OnClickAction();

	void OngoinAction();

	void OnResetAction();

	void OpenBackpack();
	void SwitchingItem(const FInputActionValue& Value);
	void DropItem();

	void Initializer();

public:
	FOnSwitchMainHand OnSwitchMainHand;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstCamera;

	//UPROPERTY(VisibleAnywhere, Category = "Camera")
	//UCameraComponent* FreeCamera;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Interactive", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractiveComponent> InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Interactive", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingComponent> CraftingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBackpackComponent> BackpackComponent;

private:
	enum class EPerspective : uint8
	{
		First,
		Third,
		Free
	};

	EPerspective NextPerspective = EPerspective::Third;

	UPROPERTY()
	int32 MainHandIndex = 0;

public:
	FORCEINLINE UCraftingComponent* GetCraftingComponent() const { return CraftingComponent; }
	FORCEINLINE UBackpackComponent* GetBackpackComponent() const { return BackpackComponent; }
};
