// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MinecraftSettings.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "MinecraftSettings"))
class MINECRAFT_API UMinecraftSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return TEXT("MinecraftSettings"); }
	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override { return TEXT("MinecraftSettings"); }

public:
	UMinecraftSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	int32 ChunkSize;

	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly)
	int32 ChunkAear;

	// 用途：允许在不需要立即加载资源时引用它们。通过存储资源的路径而不是直接的对象指针，TSoftObjectPtr 可以在需要时灵活地加载资源，减少加载负担并缩短启动时间。
	// 软引用不会主动加载资源，只在使用时才会加载。
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> BlockDataTable;

private:
#if WITH_EDITOR
	void OnChangedProperty(UObject* Object, struct FPropertyChangedEvent& Event);
#endif
};
