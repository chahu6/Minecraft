#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "Framework/Docking/TabManager.h"

class ULandscapeExtraTerrain;
/**
 * 
 */
class LANDSCAPEEXTRA_API FLandscapeExtraTerrainEditor : public FWorkflowCentricApplication
{
public:
	// 必须实现的接口
	virtual FName GetToolkitFName() const override { return FName("OurAssetsEditorToolkit"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("EditorExtension", "Out Asset Toolkit Name", "我们的资产编辑器"); }
	virtual FString GetWorldCentricTabPrefix() const override { return NSLOCTEXT("EditorExtension", "Out Asset Toolkit Tab Prefix", "我们的资产").ToString(); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Green; }

	// 必须实现的虚函数
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	// 这个函数并不是虚函数，也不含有模式匹配，为公开函数被外部调用
	void InitializeAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, ULandscapeExtraTerrain* InAssets);

	// @todo This is a hack for now until we reconcile the default toolbar with application modes
	virtual void RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager);

	TSharedPtr<class FLandscapeExtraEditorToolbar> GetToolbarBuilder() { return Toolbar; }

	bool IsEditingSingleAsset() const;

	virtual UObject* GetAssetObj() const;

	ULandscapeExtraTerrain* GetLandscapeExtraTerrain() { return TerrainAsset.Get(); }

	/** Called at the end of RegenerateMenusAndToolbars() */
	virtual void PostRegenerateMenusAndToolbars() override;


	void UpdateNoiseMap(float Width, float Height, const TArray<float>& NewNoiseMap);

	void RequestUpdateNoiseMap();

	void CreateInternalWidgets();

	/**
	* 生成Widget
	*/
	TSharedRef<SWidget> SpawnNoiseProperties();

	TSharedRef<SWidget> SpawnPreview();

	TSharedRef<SWidget> SpawnNoiseMap();

	TSharedRef<SWidget> SpawnColorGradient();

	TSharedRef<SWidget> SpawnNoisePlot();

private:
	void OnAssetChanged();

protected:
	/** The toolbar builder class */
	TSharedPtr<class FLandscapeExtraEditorToolbar> Toolbar;

private:
	// 生成细节面板
	TSharedRef<SDockTab> SpawnDetailTab(const FSpawnTabArgs& SpawnTabArgs) const;

	//TObjectPtr<UEdGraph> EdGraph;
	TWeakObjectPtr<class ULandscapeExtraTerrain> TerrainAsset;

	TSharedPtr<struct FSlateMaterialBrush> NoiseMap;
	TSharedPtr<IDetailsView> DetailsView;

	FCriticalSection ThreadCritical;
};
