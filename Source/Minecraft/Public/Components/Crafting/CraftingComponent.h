#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemStack.h"
#include "Types/BlockType.h"
#include "CraftingComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCraftingItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCraftingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void CreateItem(int32 Index, const FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable)
	bool TransferSlot(int32 Index, const FItemStack& HangItemStack, FItemStack& NewItemStack);

	UFUNCTION(BlueprintCallable)
	void CreateItemOutput(const FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(int32 Index);

	bool IsValidIndex(int32 Index);

	FItemStack GetItem(int32 Index);

	void SetItem(const FItemStack& ItemStack, int32 Index);

	void IncreaseItemAmount(int32 Index);

	void DecreaseItemAmount(int32 Index);

	bool TryAddItem(const FItemStack& ItemStack, int32 Index);

public:
	FOnCraftingItem OnCraftingItemStart;
	FOnCraftingItem OnCraftingItemCompleted;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	int32 Dimension = 3;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> Items;

	static TMap<FString, FItemStack> Recipes;

public:
	FORCEINLINE int32 GetSize() const { return Dimension; }
	FORCEINLINE void SetSize(int32 NewSize) { Dimension = NewSize; Items.SetNum(Dimension * Dimension); }
};
