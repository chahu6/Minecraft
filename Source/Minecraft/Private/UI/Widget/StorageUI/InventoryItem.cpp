#include "UI/Widget/StorageUI/InventoryItem.h"
#include "Interfaces/InventoryInterface.h"

FItemStack UInventoryItem::GetItemStack() const
{
    return IInventoryInterface::Execute_GetItemStack(Inventory.GetObject(), Index);
}

FItemStack UInventoryItem::DecrStackSize(int32 Amount)
{
    return IInventoryInterface::Execute_DecrStackSize(Inventory.GetObject(), Index, Amount);
}

void UInventoryItem::Grow(int32 Count)
{
    FItemStack ItemStack = GetItemStack();
    ItemStack.Grow(Count);
    SetInventorySlotContents(ItemStack);
}

void UInventoryItem::SetInventorySlotContents(const FItemStack& InItemStack)
{
    IInventoryInterface::Execute_SetInventorySlotContents(Inventory.GetObject(), Index, InItemStack);
}

bool UInventoryItem::IsEmpty() const
{
    return GetItemStack().IsEmpty();
}
