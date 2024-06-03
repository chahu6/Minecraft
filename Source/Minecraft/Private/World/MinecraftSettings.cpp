// Fill out your copyright notice in the Description page of Project Settings.


#include "World/MinecraftSettings.h"

UMinecraftSettings::UMinecraftSettings()
{
	ChunkSize = 16;
	ChunkAear = ChunkSize * ChunkSize;

#if WITH_EDITOR
	SettingsChangedDelegate.AddUObject(this, &UMinecraftSettings::OnChangedProperty);
#endif
}

#if WITH_EDITOR
void UMinecraftSettings::OnChangedProperty(UObject* Object, struct FPropertyChangedEvent& Event)
{
	FName PropertyName = Event.Property != nullptr ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UMinecraftSettings, ChunkSize))
	{
		ChunkAear = ChunkSize * ChunkSize;
	}
}
#endif