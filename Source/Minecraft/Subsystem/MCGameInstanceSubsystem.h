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

public:
	FORCEINLINE UDataTable* GetBlockDataTable() const { return BlockDataTable; }
};
