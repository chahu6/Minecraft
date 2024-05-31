#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemStack.h"
#include "Interfaces/InventoryInterface.h"
#include "BackpackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

class ADroppedItem;
struct FItemStack;
class UBackpack;

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
	bool TransferSlots(FItemStack HangItemStack, FItemStack& NewHangItemStack, UBackpackComponent* DestinationInventory, int32 DestinationIndex);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(int32 Index);

	void ConsumeItemStack();

	static bool IsHotbarSlot(int32 Index);

private:

	/*
	* 通知UI更新
	*/
	void NotifyAndUpdateUI(int32 Index);

	/*
	* 背包数据更新后所作的事情
	*/
	void AfterDataUpdate(int32 Index);

public:
	FOnInventoryUpdate OnInventoryUpdate;
	FOnInventoryUpdate OnHotbarUpdate;
	FOnInventoryUpdate OnArmorUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> Items;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ArmorItems;
	
	FItemStack OffHand;
	
	UPROPERTY()
	AMCPlayer* Player = nullptr;

	int32 HotbarSelectedIndex = 0;

public:
	FORCEINLINE const TArray<FItemStack>& GetItems() const { return Items; }
	FORCEINLINE FItemStack GetItemStack(int32 Index) const { return Items.IsValidIndex(Index) ? Items[Index] : FItemStack(); }
	FORCEINLINE bool SetItemStack(int32 Index, const FItemStack& NewItemStack);
};
