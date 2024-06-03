// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Items.h"

UItems* UItems::Get()
{
    if (GEngine)
    {
        static UItems* Instance = Cast<UItems>(GEngine->GameSingleton);
        return Instance;
    }
    return nullptr;
}
