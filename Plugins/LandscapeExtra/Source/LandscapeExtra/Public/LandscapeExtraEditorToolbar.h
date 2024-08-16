#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LANDSCAPEEXTRA_API FLandscapeExtraEditorToolbar : public TSharedFromThis<FLandscapeExtraEditorToolbar>
{
public:
	FLandscapeExtraEditorToolbar(TSharedPtr<class FLandscapeExtraTerrainEditor> InLandscapeExtraTerrainEditor);

	void AddCompileToolbar(UToolMenu* InMenu);

	void AddLandscapeExtraEditorModesToolbar(TSharedPtr<FExtender> Extender);

	void FillLandscapeExtraEditorModesToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	TWeakPtr<FLandscapeExtraTerrainEditor> LandscapeExtraTerrainEditor;
};
