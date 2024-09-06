#pragma once

#include "CoreMinimal.h"
#include "Item/ItemInfo.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "CraftingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCraftingItem);

struct FItemOutput
{
	int32 ItemID = 0;

	int32 Quantity = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UCraftingComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

	static TMap<FString, FItemOutput> ItemRecipes;

public:	
	UCraftingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	static void InitialItemRecipes();

	/** Inventory Interface */
	virtual void TryAddItem_Implementation(int32 Index, FItemData& InItemData) override;
	virtual void RemoveItem_Implementation(int32 Index, FItemData& OutItemData) override;
	virtual void TransferItem_Implementation(int32 Index, FItemData& OutItemData) override;
	/** End Inventory Interface */

	void MakeRecipe();

	FItemOutput GetRecipeOutput(const FString& Formula);

	void IncreaseItemAmount(int32 Index);

	void DecreaseItemAmount(int32 Index);

public:
	FOnCraftingItem OnCraftingItem;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	int32 Dimension = 3;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> Items;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FItemData OutputItemData;

public:
	FORCEINLINE int32 GetSize() const { return Dimension; }
	FORCEINLINE void SetSize(int32 NewSize) { Dimension = NewSize; Items.SetNum(Dimension * Dimension); }
};
