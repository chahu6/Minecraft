#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hotbar.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UHotbar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FlushHotbar();

	UFUNCTION(BlueprintImplementableEvent)
	void FlushSelectedBox(int32 Index);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class UBackpackComponent* Backpack;
};
