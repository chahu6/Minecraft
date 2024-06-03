// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemStack.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINECRAFT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	bool AddItemToInventory(FItemStack& ItemStack);

	UFUNCTION(BlueprintCallable)
	bool TransferSlots(FItemStack HangItemStack, FItemStack& NewHangItemStack, UInventoryComponent* DestinationInventory, int32 DestinationIndex);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemFromInventory(int32 Index);

	FItemStack GetItemStack(int32 Index);
	bool SetItemStack(int32 Index, const FItemStack& NewItemStack);

protected:
	virtual void BeginPlay() override;

	bool IsValidIndex(int32 Index);
		
protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemStack> ItemsData;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventorySize = 9;
};
