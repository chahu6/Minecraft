// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshWidget.generated.h"

class USlateVectorArtData;
class SStaticMeshWidget;
/**
 * 
 */
UCLASS()
class MINECRAFT_API UMeshWidget : public UWidget
{
	GENERATED_BODY()
public:
	UMeshWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USlateVectorArtData> MeshData;

	virtual void SynchronizeProperties() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	// Begin UWidget Interface
	//virtual const FSlateBrush* GetEditorIcon() override;
	virtual const FText GetPaletteCategory() override;
	virtual void OnCreationFromPalette() override;
	// End UWidget Interface
#endif

protected:
	// Native Slate Widget
	TSharedPtr<SStaticMeshWidget> MyMeshWidget;

	// UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
};