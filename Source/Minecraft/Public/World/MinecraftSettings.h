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

	// ��;�������ڲ���Ҫ����������Դʱ�������ǡ�ͨ���洢��Դ��·��������ֱ�ӵĶ���ָ�룬TSoftObjectPtr ��������Ҫʱ���ؼ�����Դ�����ټ��ظ�������������ʱ�䡣
	// �����ò�������������Դ��ֻ��ʹ��ʱ�Ż���ء�
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> BlockDataTable;

private:
#if WITH_EDITOR
	void OnChangedProperty(UObject* Object, struct FPropertyChangedEvent& Event);
#endif
};
