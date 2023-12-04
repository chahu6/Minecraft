#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Minecraft/Item/Info/ItemInfo.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FlushInventory();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class AMCCharacter* Player;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComp;

};
