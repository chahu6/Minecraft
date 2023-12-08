#pragma once

#include "CoreMinimal.h"
#include "FoodProperties.h"

/**
 * 
 */
class MINECRAFT_API Item
{
	class Properties
	{
		friend class Item;

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
		int32 maxDamage;
		TSharedPtr<FoodProperties> foodProperties;
	};

public:
	Item() = default;
	Item(Properties properties);
	virtual ~Item();

private:
	// ����;ö�
	int32 maxDamage; 

	// ���ѵ����� 0�����ɶѵ�
	int32 maxStackSize;

	TSharedPtr<FoodProperties> foodProperties;
};
