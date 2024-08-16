#include "AssetTypeActions_Terrain.h"
#include "LandscapeExtraTerrain.h"
#include "LandscapeExtraTerrainEditor.h"

FAssetTypeActions_Terrain::FAssetTypeActions_Terrain(EAssetTypeCategories::Type InAssetCategory)
	:AssetCategory(InAssetCategory)
{
}

UClass* FAssetTypeActions_Terrain::GetSupportedClass() const
{
	return ULandscapeExtraTerrain::StaticClass();
}

FText FAssetTypeActions_Terrain::GetName() const
{
	//return INVTEXT("LandscapeExtra Terrain");

	// ±¾µØ»¯
	return NSLOCTEXT("Editor", "LandscapeExtraTerrain", "LandscapeExtraTerrain");
}

FColor FAssetTypeActions_Terrain::GetTypeColor() const
{
	return FColor::Orange;
}

uint32 FAssetTypeActions_Terrain::GetCategories()
{
	return AssetCategory;
}

void FAssetTypeActions_Terrain::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	MenuBuilder.AddMenuEntry(
		FText::FromString("Custom Action"),
		FText::FromString("This is a custom action"),
		FSlateIcon(),
		FUIAction()
	);
}

bool FAssetTypeActions_Terrain::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FAssetTypeActions_Terrain::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	//FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);

	const EToolkitMode::Type ToolKitModeType = EditWithinLevelEditor ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjectIterator = InObjects.CreateConstIterator(); ObjectIterator; ++ObjectIterator)
	{
		if (ULandscapeExtraTerrain* OurAsset = Cast<ULandscapeExtraTerrain>(*ObjectIterator))
		{
			const TSharedRef<FLandscapeExtraTerrainEditor> RecoilAssetEditorToolKit = MakeShareable(new FLandscapeExtraTerrainEditor());
			RecoilAssetEditorToolKit->InitializeAssetEditor(ToolKitModeType, EditWithinLevelEditor, OurAsset);
		}
	}
}
