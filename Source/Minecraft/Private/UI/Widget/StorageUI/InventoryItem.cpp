#include "UI/Widget/StorageUI/InventoryItem.h"
#include "Interfaces/InventoryInterface.h"

FItemStack UInventoryItem::GetItemStack() const
{
    return IInventoryInterface::Execute_GetItemStack(Inventory.GetObject(), Index);
}
