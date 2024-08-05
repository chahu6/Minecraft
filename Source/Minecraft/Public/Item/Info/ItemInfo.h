#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/ItemType.h"
#include "ItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FItemInstance : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "64", EditCondition = "bIsStack"))
	uint8 MaxCount = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Discription = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType Type = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon = nullptr;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "64", EditCondition = "bIsStack"))
	uint8 MaxCount = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name = TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Discription = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType Type = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon = nullptr;

	void Clear()
	{
		ID = 0;
		Quantity = 0;
	}

	bool IsValid()
	{
		return ID > 0;
	}
};