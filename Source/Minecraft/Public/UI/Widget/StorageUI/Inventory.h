#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Info/ItemInfo.h"
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

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FlushInventory();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class APawn* Player;

	//UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	//class UInventoryComponent* InventoryComp;

};
