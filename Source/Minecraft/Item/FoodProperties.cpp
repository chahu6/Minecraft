#include "FoodProperties.h"

FoodProperties::FoodProperties(int32 Nutrition, bool bIsMeat, bool bCanAlwaysEat, bool bFastFood)
{
	this->Nutrition = Nutrition;
	this->bIsMeat = bIsMeat;
	this->bCanAlwaysEat = bCanAlwaysEat;
	this->bFastFood = bFastFood;
}

FoodProperties::~FoodProperties()
{
}
