// Copyright Epic Games, Inc. All Rights Reserved.

#include "LandscapeExtra.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_Terrain.h"

#include "Widgets/Layout/SConstraintCanvas.h"

#define LOCTEXT_NAMESPACE "FLandscapeExtraModule"

void FLandscapeExtraModule::StartupModule()
{
	//IAssetTools& AssetToolModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get(); // Ҳ��������д
	EAssetTypeCategories::Type Category = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(FName(TEXT("Example")), NSLOCTEXT("Editor", "LandscapeExtraTerrain", "Example"));
	TerrainActions = MakeShared<FAssetTypeActions_Terrain>(Category);
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		TerrainActions.ToSharedRef());

	//CreateButtonByStyle();
}

void FLandscapeExtraModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(TerrainActions.ToSharedRef());
	}

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("MainLayoutTab"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.1"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.2"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.3"));
}

void FLandscapeExtraModule::CreateButtonByStyle()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");

	FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);
	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		"Custom Slate Button",
		FToolUIActionChoice(FExecuteAction::CreateRaw(this, &FLandscapeExtraModule::ButtonAction)),
		FText::FromString("Custom Slate Button"),
		FText::FromString("This is a custom slate button."),
		FSlateIcon()
	));
}

void FLandscapeExtraModule::ButtonAction()
{
	UE_LOG(LogTemp, Warning, TEXT("OK"));

	SpawnLayoutTab();
}

void FLandscapeExtraModule::SpawnLayoutTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("MainLayoutTab"),
		FOnSpawnTab::CreateRaw(this, &FLandscapeExtraModule::FOnSpawnMainTab))
		.SetDisplayName(FText::FromString("Layout Tab"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.1"),
		FOnSpawnTab::CreateRaw(this, &FLandscapeExtraModule::FOnSpawnSubTab,
			FText::FromString("This is Sub-Tab 1.")))
		.SetDisplayName(FText::FromString("SubTab 1"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.2"),
		FOnSpawnTab::CreateRaw(this, &FLandscapeExtraModule::FOnSpawnSubTab,
			FText::FromString("This is Sub-Tab 2.")))
		.SetDisplayName(FText::FromString("SubTab 2"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.3"),
		FOnSpawnTab::CreateRaw(this, &FLandscapeExtraModule::FOnSpawnSubTab,
			FText::FromString("This is Sub-Tab 3.")))
		.SetDisplayName(FText::FromString("SubTab 3"));

	TSharedPtr<SDockTab> ExistingTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("MainLayoutTab"));

	if (!ExistingTab.IsValid())
	{
		// ���û���Ѵ��ڵ�Tab������Tab
		FGlobalTabmanager::Get()->TryInvokeTab(FName("MainLayoutTab"));
	}
	else
	{
		// ���Tab�Ѵ��ڣ��ر�Tab
		ExistingTab->RequestCloseTab();
	}
}

TSharedRef<SDockTab> FLandscapeExtraModule::FOnSpawnMainTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> MainTab = SNew(SDockTab).TabRole(ETabRole::NomadTab);

	if (!TabManager.IsValid())
	{
		// �½�һ��ר�õ�TabManager�����ǽ������TabManager���ָ�����
		TabManager = FGlobalTabmanager::Get()->NewTabManager(MainTab);
	}

	if (!TabManagerLayout.IsValid())
	{
		// ���ɲ���
		TabManagerLayout = FTabManager::NewLayout("AliceTabManagerLayout")
			->AddArea
			(
				FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Horizontal)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(.4f)
						->AddTab("SubTab.1", ETabState::OpenedTab)
						->SetHideTabWell(false)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(.4f)
						->AddTab("SubTab.2", ETabState::OpenedTab)
						->SetHideTabWell(false)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(.4f)
						->AddTab("SubTab.4", ETabState::OpenedTab)
						->SetHideTabWell(false)
					)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(.4f)
					->AddTab("SubTab.3", ETabState::OpenedTab)
					->SetHideTabWell(false)
				)
			);
	}

	// �ָ�����
	TSharedRef<SWidget> TabContents = TabManager->RestoreFrom(
		TabManagerLayout.ToSharedRef(), TSharedPtr<SWindow>()).ToSharedRef();

	MainTab->SetContent
	(
		TabContents
	);

	return MainTab;
}

TSharedRef<SDockTab> FLandscapeExtraModule::FOnSpawnSubTab(const FSpawnTabArgs& Args, FText Text)
{
	TSharedRef<SDockTab> Tab = SNew(SDockTab).TabRole(ETabRole::NomadTab);

	Tab->SetContent
	(
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Alignment(0.5f)
		.Anchors(0.5f)
		.Offset(FMargin(0.0f, .0f, 300.0f, 100.0f))
		.AutoSize(true)
		[
			SNew(STextBlock)
				.Text(Text)
		]
	);

	return Tab;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLandscapeExtraModule, LandscapeExtra)