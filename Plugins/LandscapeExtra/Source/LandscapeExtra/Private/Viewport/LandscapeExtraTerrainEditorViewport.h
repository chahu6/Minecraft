#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"

class SLandscapeExtraTerrainEditorViewport : public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SLandscapeExtraTerrainEditorViewport) {}
	SLATE_ATTRIBUTE(class ULandscapeExtraTerrain*, TerrainAsset)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//Toolbar interface
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;

protected:
	/** Begin SViewport Interface */

	// 这个函数实现聚焦的时候的行为，也就是按F该干啥，不会有人用手柄开发游戏吧
	virtual void OnFocusViewportToSelection() override;

	// 添加一个ViewportClient类，我们要处理的核心部分之一
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

	// 创建Toolbar
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

private:
	class ULandscapeExtraTerrain* TerrainAsset;
	TSharedPtr<class FLandscapeExtraTerrainEditorViewportClient> TerrainEditorViewportClient;
	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;
};