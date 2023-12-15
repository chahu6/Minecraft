#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None", ToolTip = "��"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock", ToolTip = "��������"),
	NaturalBlock	UMETA(DisplayName = "NaturalBlock", ToolTip = "��Ȼ����"),
	Tool			UMETA(DisplayName = "Tool", ToolTip = "����"),
	Combat			UMETA(DisplayName = "Combat", ToolTip = "ս������"),
	Consumable		UMETA(DisplayName = "Consumable", ToolTip = "����Ʒ"),
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
	int32 MaxCount = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UStaticMesh* Mesh = nullptr;

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