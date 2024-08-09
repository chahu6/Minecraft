// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
};
