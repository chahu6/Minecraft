#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Container.h"
#include "Item/ItemStack.h"
#include "Inventory.generated.h"

class UUniformGridPanel;
class UBackpackComponent;
class UInventoryItem;
class UDroppableInventoryCellWidget;
class UCanvasPanel;

class UCraftingResultComponent;
class UCraftingComponent;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UInventory : public UContainer
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void InitUI();

	UFUNCTION(BlueprintCallable, Category = "Player Inventory")
	void FlushHotbar();

	UFUNCTION(BlueprintCallable, Category = "Player Inventory")
	void FlushInventory();

	UFUNCTION(Category = "Player Inventory")
	void InitHotbarWidget();

	UFUNCTION(Category = "Player Inventory")
	void InitInventoryWidget();

	void RemoveDroppableItemWidget();

	virtual void HandleLMB_Implementation(UInventoryItem* InventoryItem) override;
	virtual void HandleRMB_Implementation(UInventoryItem* InventoryItem) override;

	virtual void OnHandleLMB(UInventoryItem* InventoryItem);

	virtual void OnCraftMatrixChanged();
	void SlotChangedCraftingGrid(AEntityPlayer* PlayerIn, UCraftingComponent* CraftingMatrix, UCraftingResultComponent* CraftingResult);

	void CreateDroppableItemWidget(const FItemStack& ItemStack);

	void UpdateDroppableItemWidget();

protected:
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInventoryItem> InventoryItemClass;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UDroppableInventoryCellWidget> DroppableInventoryCellWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDroppableInventoryCellWidget> DroppableItem;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Inventory;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Hotbar;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBackpackComponent> BackpackComponent;

	UPROPERTY(BlueprintReadOnly)
	FVector2D HangImageSize = { 100, 100 };

	UPROPERTY(BlueprintReadOnly)
	bool bIsDragInProgress;

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 DragFromCellIndex;

	UPROPERTY(BlueprintReadOnly, Transient)
	FItemStack HangItemStack;
};
