#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Minecraft/Item/Info/ItemInfo.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);

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
	bool AddItemToInventory(int32 ID, int32 Num);

	UFUNCTION(BlueprintCallable)
	void TransferSlots(int32 SourceIndex, UInventoryComponent* SourceInventory, int32 DestinationIndex);

private:
	bool FindItemIndex(int32 ID, int32& Index);
	bool AnyEmptySlots(int32& Index);

public:
	UPROPERTY(BlueprintAssignable, Category = "Event", meta = (AllowPrivateAccess = "true"))
	FOnInventoryUpdate OnInventoryUpdate;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemSlot> Items;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventorySize = 27;


public:
	FORCEINLINE TArray<FItemSlot>& GetItems() { return Items; }
};
