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
	// ����ʵ�ֵĽӿ�
	virtual FName GetToolkitFName() const override { return FName("OurAssetsEditorToolkit"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("EditorExtension", "Out Asset Toolkit Name", "���ǵ��ʲ��༭��"); }
	virtual FString GetWorldCentricTabPrefix() const override { return NSLOCTEXT("EditorExtension", "Out Asset Toolkit Tab Prefix", "���ǵ��ʲ�").ToString(); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Green; }

	// ����ʵ�ֵ��麯��
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	// ��������������麯����Ҳ������ģʽƥ�䣬Ϊ�����������ⲿ����
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
	* ����Widget
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
	// ����ϸ�����
	TSharedRef<SDockTab> SpawnDetailTab(const FSpawnTabArgs& SpawnTabArgs) const;

	//TObjectPtr<UEdGraph> EdGraph;
	TWeakObjectPtr<class ULandscapeExtraTerrain> TerrainAsset;

	TSharedPtr<struct FSlateMaterialBrush> NoiseMap;
	TSharedPtr<IDetailsView> DetailsView;

	FCriticalSection ThreadCritical;
};
