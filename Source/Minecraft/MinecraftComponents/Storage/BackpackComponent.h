#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/Item/ItemStack.h"
#include "Minecraft/Interfaces/InventoryInterface.h"
#include "BackpackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

class ADroppedItem;

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
	FItemStack GetSelected(int32 SelectedIndex);

	bool AddItemToInventory(const ADroppedItem* DroppedItem);

	UFUNCTION(BlueprintCallable)
	bool TransferSlots(int32 SourceIndex, UBackpackComponent* SourceInventory, int32 DestinationIndex);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(int32 Index);

	static bool IsHotbarSlot(int32 Index);

private:
	void NotifyAndUpdateUI(int32 Index);

public:
	FOnInventoryUpdate OnInventoryUpdate;
	FOnInventoryUpdate OnHotbarUpdate;
	FOnInventoryUpdate OnArmorUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> Items;
	
	/*UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> HotbarItems;*/

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ArmorItems;
	
	FItemStack OffHand;
	
	UPROPERTY()
	AMCPlayer* Player;

public:
	FORCEINLINE const TArray<FItemStack>& GetItems() const { return Items; }
	FORCEINLINE bool SetItemStack(int32 Index, const FItemStack& NewItemStack);
};
