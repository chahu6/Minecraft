// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MeshWidget.h"
#include "UI/Widget/SStaticMeshWidget.h"

#define LOCTEXT_NAMESPACE "UMG"

UMeshWidget::UMeshWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMeshWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyMeshWidget.IsValid() && MeshData != nullptr)
	{
		const uint32 NewMeshIndex = MyMeshWidget->AddStaticMesh(*MeshData);
	}
}

void UMeshWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMeshWidget.Reset();
}

TSharedRef<SWidget> UMeshWidget::RebuildWidget()
{
	MyMeshWidget = SNew(SStaticMeshWidget);
	return MyMeshWidget.ToSharedRef();
}

#if WITH_EDITOR
const FText UMeshWidget::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

void UMeshWidget::OnCreationFromPalette()
{
}
#endif

#undef LOCTEXT_NAMESPACE