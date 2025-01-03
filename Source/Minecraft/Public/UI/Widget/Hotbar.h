#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/View.h"
#include "Hotbar.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UHotbar : public UView
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FlushHotbar();
};
