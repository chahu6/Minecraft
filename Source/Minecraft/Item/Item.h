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
	// ����;ö�
	int32 MaxDamage; 

	// ���ѵ����� 0�����ɶѵ�
	int32 MaxStackSize;
};
