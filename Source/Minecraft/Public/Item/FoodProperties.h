#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MINECRAFT_API FoodProperties
{
public:
	FoodProperties(int32 Nutrition, bool bIsMeat, bool bCanAlwaysEat, bool bFastFood);
	~FoodProperties();

private:
	int32 Nutrition; // ÓªÑø

	bool bIsMeat;

	bool bCanAlwaysEat;

	bool bFastFood;
};
