#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BlockID.h"
#include "Block.generated.h"

class UBlockBehavior;

struct FBlockData
{
	EBlockID ID = EBlockID::Air;
	uint8 BlockState = 0;

	FBlockData() : ID(EBlockID::Air), BlockState(0) {}

	FBlockData(EBlockID InBlockID) : ID(InBlockID) {}

	FBlockData(EBlockID InBlockID, uint8 InBlockState)
		:ID(InBlockID), BlockState(InBlockState)
	{}
};

USTRUCT(BlueprintType)
struct FBlockMeta : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BlockID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BlockName = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* Material = nullptr;

	// 是否是流体
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLiquid = false;

	// 是否是透明的
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bTransparent = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Hardness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBlockBehavior> BehaviorClass = nullptr;
};