#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Inventory.h"
#include "Backpack.generated.h"

class UBackpackComponent;
class UCraftingComponent;
class UCraftingResultComponent;
class UInventoryItem;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBackpack : public UInventory
{
	GENERATED_BODY()

public:
	virtual void OnContainerClosed(AEntityPlayer* PlayerIn) override;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	virtual void InitUI() override;

	UFUNCTION(BlueprintImplementableEvent)
	void FlushCrafting();

	virtual void OnCraftMatrixChanged() override;

	virtual void OnHandleLMB(UInventoryItem* InventoryItem) override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInventoryItem> Armor_Helmet;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInventoryItem> Armor_Chestplate;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInventoryItem> Armor_Leggings;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInventoryItem> Armor_Boots;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInventoryItem> Off_Hand;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingComponent> CraftingSystem;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingResultComponent> CraftingResult;
};
