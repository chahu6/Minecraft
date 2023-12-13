#pragma once

#include "CoreMinimal.h"
#include "FoodProperties.h"
#include "IItemType.h"

class FBlock;
/**
 * 
 */
class MINECRAFT_API FItem : public IItemType
{
public:
	class Properties
	{
		friend class FItem;

	public:
		Properties* food(const TSharedPtr<FoodProperties>& newFoodProperties)
		{
			this->foodProperties = newFoodProperties;
			return this;
		}

		Properties* stacksTo(int32 stackSize)
		{
			ensure(this->maxDamage <= 0);

			this->maxStackSize = stackSize;
			return this;
		}

		Properties* defaultDurability(int32 durability)
		{
			return this->maxDamage == 0 ? this->durability(durability) : this;
		}

		Properties* durability(int32 durability)
		{
			this->maxDamage = durability;
			this->maxStackSize = 1;
			return this;
		}

	private:
		int32 maxStackSize = 64;
		int32 maxDamage = 0;
		TSharedPtr<FoodProperties> foodProperties = nullptr;
	};

public:
	FItem() = default;
	FItem(Properties properties);

public:
	static TMap<TSharedPtr<FBlock>, TSharedPtr<FItem>> BLOCK_ITEM;

	//static GetItemFromBlock(Block* blockIn);

	virtual EType GetItemType() const override { return EType::Item; }
private:
	// ����;ö�
	int32 maxDamage; 

	// ���ѵ����� 0�����ɶѵ�
	int32 maxStackSize;

	TSharedPtr<FoodProperties> foodProperties;
};
