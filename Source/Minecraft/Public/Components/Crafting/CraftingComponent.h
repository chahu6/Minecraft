#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStack.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "CraftingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCraftingItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UCraftingComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()
public:	
	UCraftingComponent();
	virtual void BeginPlay() override;

	/** Inventory Interface */
	virtual int32 GetSizeInventory_Implementation() const override;
	virtual bool IsEmpty_Implementation() const override;
	virtual bool IsEmptyFromIndex_Implementation(int32 Index) const override;
	virtual FItemStack GetItemStack_Implementation(int32 Index) const override;
	virtual FItemStack DecrStackSize_Implementation(int32 Index, int32 Count) override;
	virtual FItemStack RemoveStackFromSlot_Implementation(int32 Index) override;
	virtual void SetInventorySlotContents_Implementation(int32 Index, const FItemStack& Stack) override;
	virtual bool AddItemToInventoryFromIndex_Implementation(int32 Index, FItemStack& InItemStack);
	virtual void Clear_Implementation() override;
	/** end Inventory Interface */

	FItemStack GetStackInRowAndColumn(int32 Row, int32 Column);

	UFUNCTION(BlueprintCallable)
	void ShrinkAllItems();

	void Init();

protected:
	void NotifyAndUpdate();

	void OnCraftMatrixChanged();

public:
	FOnCraftingItem OnCraftingItem;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	int32 InventoryWidth = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	int32 InventoryHeight = 2;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> StackList;

public:
	FORCEINLINE void SetInventoryWidth(int32 InInventoryWidth) { InventoryWidth = InInventoryWidth; }
	FORCEINLINE void SetInventoryHeight(int32 InInventoryHeight) { InventoryHeight = InInventoryHeight; }
};
