// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/NBT/KNBTTagArray.h"

template<typename T>
inline ENBTTagType KNBTTagArray<T>::GetId() const
{
	check(false);
	return ENBTTagType::TERMINAL;
}

template<>
inline ENBTTagType KNBTTagArray<uint8>::GetId() const
{
	return ENBTTagType::BYTE_ARRAY;
}

template<>
inline ENBTTagType KNBTTagArray<int16>::GetId() const
{
	return ENBTTagType::INT16_ARRAY;
}

template<>
inline ENBTTagType KNBTTagArray<int32>::GetId() const
{
	return ENBTTagType::INT32_ARRAY;
}
