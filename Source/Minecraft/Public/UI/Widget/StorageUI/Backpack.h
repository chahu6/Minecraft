#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemInfo.h"
#include "Backpack.generated.h"

class UBackpackComponent;
class UCraftingComponent;
class UInventoryItem;

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBackpack : public UUserWidget
{
	GENERATED_BODY()

public:
	UBackpack(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	void InitUI();

	UFUNCTION(BlueprintImplementableEvent)
	void FlushBackpack();

	UFUNCTION(BlueprintImplementableEvent)
	void FlushHotbar();

	UFUNCTION(BlueprintImplementableEvent)
	void FlushCrafting();

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
	TObjectPtr<UBackpackComponent> BackpackComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingComponent> CraftingSystem;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APawn> Player;

	UPROPERTY(BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	FItemData HangItemData;
};
