#include "Components/Crafting/CraftingComponent.h"
#include "World/Block/BlockID.h"
#include "Item/ItemID.h"
#include "Utils/MinecraftAssetLibrary.h"

TMap<FString, FItemOutput> UCraftingComponent::ItemRecipes;

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Items.SetNum(Dimension * Dimension);
}

void UCraftingComponent::InitialItemRecipes()
{
	ItemRecipes.Add(FString::Printf(TEXT("A%dB*C*D*"), EBlockID::Oak_Wood), { (int32)EBlockID::Oak_Planks, 4 });
}

void UCraftingComponent::TryAddItem_Implementation(int32 Index, FItemData& InItemData)
{
	if (Items.IsValidIndex(Index) && InItemData.IsValid())
	{
		FItemData& ItemData = Items[Index];
		if (ItemData.ID > 0 && ItemData.ID == InItemData.ID && ItemData.bIsStack)
		{
			int32 Sum = ItemData.Quantity + InItemData.Quantity;
			if (ItemData.MaxCount >= Sum)
			{
				ItemData.Quantity = Sum;
				InItemData.Clear();
			}
			else
			{
				ItemData.Quantity = ItemData.MaxCount;
				InItemData.Quantity = Sum - ItemData.MaxCount;
			}
		}
		else if (ItemData.ID > 0)
		{
			FItemData TempItemData = ItemData;
			ItemData = InItemData;
			InItemData = TempItemData;
		}
		else
		{
			ItemData = InItemData;
			InItemData.Clear();
		}

		MakeRecipe();
		OnCraftingItem.Broadcast();
	}
	else
	{

	}
}

void UCraftingComponent::RemoveItem_Implementation(int32 Index, FItemData& OutItemData)
{
	if (Items.IsValidIndex(Index))
	{
		OutItemData = Items[Index];
		Items[Index].Clear();

		MakeRecipe();
		OnCraftingItem.Broadcast();
	}
	else
	{
		OutItemData.Clear();
	}
}

void UCraftingComponent::TransferItem_Implementation(int32 Index, FItemData& OutItemData)
{
	if (Items.IsValidIndex(Index))
	{
		FItemData TempItemData = OutItemData;
		OutItemData = Items[Index];
		Items[Index] = TempItemData;

		MakeRecipe();
		OnCraftingItem.Broadcast();
	}
}

void UCraftingComponent::MakeRecipe()
{
	FString Formula;

	for (int32 X = 0; X < Items.Num(); ++X)
	{
		FItemData ItemData = Items[X];
		Formula.AppendChar('A' + X);
		if (ItemData.ID > 0)
		{
			Formula.AppendInt(ItemData.ID);
		}
		else
		{
			Formula.AppendChar('*');
		}
	}

	FItemOutput ItemOutput = GetRecipeOutput(Formula);
	if (ItemOutput.ItemID > 0)
	{
		FItemInstance ItemInstance;
		if (UMinecraftAssetLibrary::GetItemInstance(ItemOutput.ItemID, ItemInstance))
		{
			OutputItemData.CopyItemInstance(ItemInstance);
			OutputItemData.Quantity = ItemOutput.Quantity;
		}
	}
	else
	{
		OutputItemData.Clear();
	}
}

FItemOutput UCraftingComponent::GetRecipeOutput(const FString& Formula)
{
	if (FItemOutput* ItemOutput = ItemRecipes.Find(Formula))
	{
		return *ItemOutput;
	}
	return {};
}

void UCraftingComponent::TryDecreaseItemAmount()
{
	for (int32 Index = 0; Index < Dimension * Dimension; ++Index)
	{
		DecreaseItemAmount(Index);
	}
}

void UCraftingComponent::IncreaseItemAmount(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index].Quantity++;
	}
}

void UCraftingComponent::DecreaseItemAmount(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		FItemData& ItemData = Items[Index];
		ItemData.Quantity--;
		if (ItemData.Quantity <= 0)
		{
			ItemData.Clear();
		}
	}
}
