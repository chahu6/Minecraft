#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/Item/ItemStack.h"
#include "Minecraft/Interfaces/InventoryInterface.h"
#include "BackpackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UBackpackComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

	friend class AMCPlayer;

public:	
	UBackpackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FItemStack GetSelected();

public:
	FOnInventoryUpdate OnInventoryUpdate;
	FOnInventoryUpdate OnHotbarUpdate;
	FOnInventoryUpdate OnArmorUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> Items;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> HotbarItems;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ArmorItems;
	
	FItemStack OffHand;
	
	UPROPERTY()
	AMCPlayer* Player;

	int32 SelectedIndex = 0;
};
