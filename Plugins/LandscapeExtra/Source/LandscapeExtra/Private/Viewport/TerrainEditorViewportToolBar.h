#pragma once

#include "CoreMinimal.h"
#include "SCommonEditorViewportToolbarBase.h"

class STerrainEditorViewportToolBar : public SCommonEditorViewportToolbarBase
{
public:
	SLATE_BEGIN_ARGS(STerrainEditorViewportToolBar) { }

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider);
};