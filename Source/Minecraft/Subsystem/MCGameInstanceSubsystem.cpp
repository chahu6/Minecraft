#include "MCGameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "Minecraft/Block/Blocks.h"

void UMCGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	BlockDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Minecraft/Blueprints/Datas/DataTable/DT_BlockDataTable.DT_BlockDataTable'"));

	//Blocks::Initial();
}
