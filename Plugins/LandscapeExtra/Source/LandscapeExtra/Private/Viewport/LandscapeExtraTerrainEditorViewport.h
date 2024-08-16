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

	// �������ʵ�־۽���ʱ�����Ϊ��Ҳ���ǰ�F�ø�ɶ�������������ֱ�������Ϸ��
	virtual void OnFocusViewportToSelection() override;

	// ���һ��ViewportClient�࣬����Ҫ����ĺ��Ĳ���֮һ
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

	// ����Toolbar
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

private:
	class ULandscapeExtraTerrain* TerrainAsset;
	TSharedPtr<class FLandscapeExtraTerrainEditorViewportClient> TerrainEditorViewportClient;
	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;
};