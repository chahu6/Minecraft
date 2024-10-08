#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

class UImage;
class UTextBlock;
class UInventoryInterface;
class UContainer;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FlushItemData();

	UFUNCTION(BlueprintCallable)
	FItemStack GetItemStack() const;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Quantity;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item", meta=(ExposeOnSpawn = true))
	TScriptInterface<UInventoryInterface> Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item", meta = (ExposeOnSpawn = true))
	TObjectPtr<UContainer> Parent;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item", meta=(ExposeOnSpawn = true))
	int32 Index;
};
