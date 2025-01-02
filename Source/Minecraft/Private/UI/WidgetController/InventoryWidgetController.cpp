// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"
#include "Item/ItemStack.h"
#include "UI/Widget/Slot.h"
#include "Components/Inventory/BackpackComponent.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
}

void UInventoryWidgetController::MouseClick(int32 ClickedIndex, EMouseEvent MouseEvent)
{
	//SlotClick(ClickedIndex, MouseEvent);
}

void UInventoryWidgetController::SlotClick(USlot* ClickedSlot, EMouseEvent MouseEvent)
{
	int32 ClickedIndex = ClickedSlot->Index;
	TScriptInterface<UInventoryInterface> InventoryInterface = ClickedSlot->InventoryInterface;

	FItemStack ClickedItemStack = IInventoryInterface::Execute_GetItemStack(InventoryInterface.GetObject(), ClickedIndex);
	FItemStack HangItemStack = Backpack->GetHangItemStack();

	if (ClickedItemStack.IsEmpty())
	{
		if (!HangItemStack.IsEmpty())
		{
			const int32 Count = HangItemStack.GetCount();
			IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, HangItemStack.SplitStack(Count));
			Backpack->SetHangItemStack(HangItemStack);
		}
	}
	else
	{
		if (HangItemStack.IsEmpty())
		{
			const int32 Count = ClickedItemStack.GetCount();
			FItemStack DecrItemStack = IInventoryInterface::Execute_DecrStackSize(InventoryInterface.GetObject(), ClickedIndex, Count);
			Backpack->SetHangItemStack(DecrItemStack);
		}
		else
		{
			if (ClickedItemStack.GetItem() == HangItemStack.GetItem())
			{
				int32 Count = HangItemStack.GetCount();

				if (Count > HangItemStack.GetMaxStackSize() - ClickedItemStack.GetCount())
				{
					Count = HangItemStack.GetMaxStackSize() - ClickedItemStack.GetCount();
				}

				HangItemStack.Shrink(Count);
				ClickedItemStack.Grow(Count);
				Backpack->SetHangItemStack(HangItemStack);
				IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, ClickedItemStack);
			}
			else
			{
				IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, HangItemStack);
				Backpack->SetHangItemStack(ClickedItemStack);
			}
		}
	}

	OnInventoryUpdateDelegate.Broadcast(ClickedIndex, IInventoryInterface::Execute_GetItemStack(InventoryInterface.GetObject(), ClickedIndex));
	OnInventoryHangItemUpdateDelegate.Broadcast(Backpack->GetHangItemStack());
}
