// Copyright Epic Games, Inc. All Rights Reserved.

#include "LanguageOne.h"
#include "LanguageOneStyle.h"
#include "LanguageOneCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "LevelEditor.h"

static const FName LanguageOneTabName("LanguageOne");

#define LOCTEXT_NAMESPACE "FLanguageOneModule"

void FLanguageOneModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLanguageOneStyle::Initialize();
	FLanguageOneStyle::ReloadTextures();

	FLanguageOneCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLanguageOneCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FLanguageOneModule::SwitchLanguage),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLanguageOneModule::RegisterMenus));

	// 注册一个窗口，目前没什么用
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		"LanguageOne",
		FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) {return SNew(SDockTab); })
	)
		.SetDisplayName(LOCTEXT("LanguageOneTab", "Switch Language"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// Register the commands with the global command list
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	LevelEditorModule.GetGlobalLevelEditorActions()->Append(PluginCommands.ToSharedRef());
}

void FLanguageOneModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FLanguageOneStyle::Shutdown();

	FLanguageOneCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("LanguageOne");
}

void FLanguageOneModule::SwitchLanguage()
{
	FString CurrentCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
	FString NewCulture = (CurrentCulture == TEXT("en")) ? TEXT("zh-Hans") : TEXT("en");

	FInternationalization::Get().SetCurrentCulture(NewCulture);

	// Refresh the UI (this might not work perfectly in all cases)
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().InvalidateAllWidgets(true);
	}

	//FGlobalTabmanager::Get()->TryInvokeTab(FName("LanguageOne"));
}

void FLanguageOneModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FLanguageOneCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FLanguageOneCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("AssetEditorToolbar.CommonActions");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FLanguageOneCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLanguageOneModule, LanguageOne)