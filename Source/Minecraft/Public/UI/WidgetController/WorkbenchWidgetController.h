// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BagWidgetController.h"
#include "WorkbenchWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UWorkbenchWidgetController : public UBagWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
};
