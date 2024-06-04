#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/MinecraftUserWidget.h"
#include "MainUI.generated.h"

class UHotbar;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UMainUI : public UMinecraftUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHotbar> Hotbar;
};
