#include "World/Block/BlockMetaDatabase.h"

UBlockMetaDatabase* UBlockMetaDatabase::SingletonManager = nullptr;

UBlockMetaDatabase::~UBlockMetaDatabase()
{
	SingletonManager = nullptr;
}

void UBlockMetaDatabase::Initializer()
{
	SingletonManager = NewObject<UBlockMetaDatabase>(GetTransientPackage(), NAME_None);
	SingletonManager->AddToRoot();
}
