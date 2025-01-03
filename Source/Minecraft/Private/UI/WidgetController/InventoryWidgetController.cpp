// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"
#include "Item/ItemStack.h"
#include "UI/Widget/Slot.h"
#include "Components/Inventory/BackpackComponent.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	BackpackComp = Pawn->GetComponentByClass<UBackpackComponent>();
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
	FItemStack HangItemStack = BackpackComp->GetHangItemStack();

	if (ClickedItemStack.IsEmpty())
	{
		if (!HangItemStack.IsEmpty() && ClickedSlot->IsItemValid(HangItemStack))
		{
			const int32 Count = MouseEvent == EMouseEvent::LMB ? HangItemStack.GetCount() : 1;
			IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, HangItemStack.SplitStack(Count));
			BackpackComp->SetHangItemStack(HangItemStack);
		}
	}
	else
	{
		if (HangItemStack.IsEmpty())
		{
			const int32 Count = MouseEvent == EMouseEvent::LMB ? ClickedItemStack.GetCount() : (ClickedItemStack.GetCount() + 1) / 2;
			FItemStack DecrItemStack = IInventoryInterface::Execute_DecrStackSize(InventoryInterface.GetObject(), ClickedIndex, Count);
			BackpackComp->SetHangItemStack(DecrItemStack);
		}
		else
		{
			if (ClickedItemStack.GetItemID() == HangItemStack.GetItemID())
			{
				int32 Count = HangItemStack.GetCount();

				if (Count > HangItemStack.GetMaxStackSize() - ClickedItemStack.GetCount())
				{
					Count = HangItemStack.GetMaxStackSize() - ClickedItemStack.GetCount();
				}

				HangItemStack.Shrink(Count);
				ClickedItemStack.Grow(Count);
				BackpackComp->SetHangItemStack(HangItemStack);
				IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, ClickedItemStack);
			}
			else
			{
				IInventoryInterface::Execute_SetInventorySlotContents(InventoryInterface.GetObject(), ClickedIndex, HangItemStack);
				BackpackComp->SetHangItemStack(ClickedItemStack);
			}
		}
	}

	OnSlotClickedUpdateDelegate.Broadcast(ClickedSlot, IInventoryInterface::Execute_GetItemStack(InventoryInterface.GetObject(), ClickedIndex));
	OnInventoryHangItemUpdateDelegate.Broadcast(BackpackComp->GetHangItemStack());
}
