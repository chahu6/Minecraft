#pragma once

#include "CoreMinimal.h"
#include "Entity/MinecraftEntity.h"
#include "InputActionValue.h"
#include "Interfaces/InteractiveInterface.h"
#include "EntityPlayer.generated.h"

class UBackpackComponent;
class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UInteractiveComponent;
class UCraftingComponent;
class UCraftingResultComponent;
class UInputAction;
class AEntityItem;

class UContainer;

UCLASS()
class MINECRAFT_API AEntityPlayer : public AMinecraftEntity, public IInteractiveInterface
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
	TObjectPtr<UInputAction> DropItemAction;

public:
	AEntityPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	void UpdateMainHandItem();

	FItemStack GetMainHandItem();

	void ConsumeItem();

	/** Interactive Interface */
	virtual bool OnItemPickup_Implementation(FItemStack& ItemStack) override;
	/** Interactive Interface end*/

	UFUNCTION(BlueprintCallable)
	void DisplayGUI(TSubclassOf<UContainer> WidgetClass, AActor* OwnerActor);

	void CloseContainer();

	AEntityItem* DropItem(bool bDropAll = false);

	UFUNCTION(BlueprintCallable, Category = "Item")
	AEntityItem* DropItem(const FItemStack& ItemStack);

private:
	void SwitchPerspectives();

	void UseItem();

	void OnClickAction();

	void OngoinAction();

	void OnResetAction();

	void OpenBackpack();

	UFUNCTION()
	void SwitchingItem(int32 WheelValue);

	void DropAction();

	void Initialization();

	void InitialInventoryUI();

	//void ToggleInventory();

	FVector GetItemSpawnLocation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float Thrust = 50000.f;

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
	TObjectPtr<USkeletalMeshComponent> ItemSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Interactive", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractiveComponent> InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Interactive", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingComponent> CraftingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Interactive", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingResultComponent> CraftingResultComponent;

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

	UPROPERTY(Transient)
	int32 MainHandIndex = 0;
};
