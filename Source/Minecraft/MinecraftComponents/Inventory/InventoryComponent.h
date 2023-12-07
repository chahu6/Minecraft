#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/Item/Info/ItemInfo.h"
#include "InventoryComponent.generated.h"

class ADroppedItem;

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool AddItemToInventory(const ADroppedItem* Item);

	UFUNCTION(BlueprintCallable)
	void TransferSlots(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex);

	UFUNCTION(BlueprintCallable)
	void CreateItemToWorld(FName ID, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(int32 Index);
private:
	bool FindItemIndex(const ADroppedItem* Item, int32& Index);
	bool AnyEmptySlots(int32& Index);

public:
	FOnInventoryUpdate OnInventoryUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemSlot> ItemsData;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventorySize = 9;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	UDataTable* ItemDataTable;

public:
	FORCEINLINE TArray<FItemSlot>& GetItems() { return ItemsData; }
	FORCEINLINE void SetInventorySize(int32 NewSize) { InventorySize = NewSize; }
};
