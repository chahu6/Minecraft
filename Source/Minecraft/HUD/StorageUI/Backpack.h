#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Backpack.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UBackpack : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FlushBackpack();

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
};
