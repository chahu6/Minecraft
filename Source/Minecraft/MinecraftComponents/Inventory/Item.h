#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(DisplayName = "None",				ToolTip = "��"),

	BuildingBlock	UMETA(DisplayName = "BuildingBlock",	ToolTip = "��������"),
	NaturalBlock	UMETA(DisplayName = "NaturalBlock",		ToolTip = "��Ȼ����"),
	Tool			UMETA(DisplayName = "Tool",				ToolTip = "����"),
	Combat			UMETA(DisplayName = "Combat",			ToolTip = "ս������"),
	Consumable		UMETA(DisplayName = "Consumable",		ToolTip = "����Ʒ"),
};

USTRUCT(BlueprintType)
struct FItemDetails : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FText Name = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FText Discription = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	bool bIsStack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 MaxCount = 64;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	EItemType Type;
};

UCLASS()
class MINECRAFT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
