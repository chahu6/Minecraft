#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Block.generated.h"

UENUM(BlueprintType)
enum class EBlockID : uint8
{
	Air,
	Stone,
	Dirt,
	Grass,
	BedRock,
	Water,

	EBT_MAX
};

USTRUCT(BlueprintType)
struct FBlockInfoTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	UMaterialInterface* Material;

	// 硬度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float Hardness;

	// 阻力系数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", Meta = (ClampMin = "0", ClampMax = "1"))
	float Friction;
};