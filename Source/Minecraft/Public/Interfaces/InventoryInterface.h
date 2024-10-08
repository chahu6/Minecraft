#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item/ItemStack.h"
#include "InventoryInterface.generated.h"

class AEntityPlayer;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINECRAFT_API IInventoryInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	int32 GetSizeInventory() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	bool IsEmpty() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	bool IsEmptyFromIndex(int32 Index) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	FItemStack GetItemStack(int32 Index) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	FItemStack DecrStackSize(int32 Index, int32 Count);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	FItemStack RemoveStackFromSlot(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void SetInventorySlotContents(int32 Index, const FItemStack& Stack);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	bool AddItemToInventoryFromIndex(int32 Index, UPARAM(ref) FItemStack& InItemStack);

	//void OpenInventory(AEntityPlayer* Player);

	//void CloseInventory(AEntityPlayer* Player);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void Clear();
};
