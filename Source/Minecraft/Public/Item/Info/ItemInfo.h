#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None", ToolTip = "无"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock", ToolTip = "建筑方块"),
	NaturalBlock	UMETA(DisplayName = "NaturalBlock", ToolTip = "自然方块"),
	Tool			UMETA(DisplayName = "Tool", ToolTip = "工具"),
	Combat			UMETA(DisplayName = "Combat", ToolTip = "战斗工具"),
	Consumable		UMETA(DisplayName = "Consumable", ToolTip = "消耗品"),
};

USTRUCT(BlueprintType)
struct FItemDetails : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FText Discription = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	bool bIsStack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag", Meta = (ClampMin = "0", ClampMax = "64", EditCondition = "bIsStack"))
	uint8 MaxCount = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	EItemType Type = EItemType::None;
};

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 MaxCount = 0;

	void Empty()
	{
		ID = -1;
		Count = 0;
		MaxCount = 0;
	}
};