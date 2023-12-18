#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Minecraft/Item/ItemStack.h"
#include "Backpack.generated.h"

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

	UFUNCTION(BlueprintCallable)
	void HangItemStackToMouse(int32 Index);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryItem* Armor_Helmet;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryItem* Armor_Chestplate;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryItem* Armor_Leggings;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryItem* Armor_Boots;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryItem* Off_Hand;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class UBackpackComponent* Backpack;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class APawn* Player;

	UPROPERTY(BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	FItemStack HangItemStack;
};
