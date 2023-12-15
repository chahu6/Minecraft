#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Quantity;
};
