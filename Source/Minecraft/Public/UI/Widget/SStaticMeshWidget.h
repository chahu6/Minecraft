#pragma once

#include "CoreMinimal.h"
#include "Slate/SMeshWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class MINECRAFT_API SStaticMeshWidget : public SMeshWidget
{
public:
	SLATE_BEGIN_ARGS(SStaticMeshWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

	uint32 AddStaticMesh(USlateVectorArtData& InMeshData);

private:
	uint32 StaticMeshIndex = -1;
};
