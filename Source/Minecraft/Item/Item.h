#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct Properties
{

};

class MINECRAFT_API Item
{
public:
	Item();
	~Item();

private:
	// 最大耐久度
	int32 MaxDamage; 

	// 最大堆叠数量 0：不可堆叠
	int32 MaxStackSize;
};
