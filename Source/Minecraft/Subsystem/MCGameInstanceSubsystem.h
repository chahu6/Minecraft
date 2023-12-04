#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMCGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY()
	class UDataTable* BlockDataTable;

	UPROPERTY()
	class UDataTable* ItemDataTable;

public:
	FORCEINLINE UDataTable* GetBlockDataTable() const { return BlockDataTable; }
	FORCEINLINE UDataTable* GetItemDataTable() const { return ItemDataTable; }
};
