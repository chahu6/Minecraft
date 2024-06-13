#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/MinecraftUserWidget.h"
#include "Hotbar.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UHotbar : public UMinecraftUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
