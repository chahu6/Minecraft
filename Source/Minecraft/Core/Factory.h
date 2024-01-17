#pragma once

#include "CoreMinimal.h"

template<typename T>
struct Factory
{
	template<typename SubT>
	struct Register_t
	{
		Register_t(const FName& Key)
		{
			if (!Factory<T>::GetInstance()._Map.Contains(Key))
			{
				Factory<T>::GetInstance()._Map.Emplace(Key, []() { return new SubT(); });
			}
		}

		template<typename... Args>
		Register_t(const FName& Key, Args&&... args)
		{
			if (!Factory<T>::GetInstance()._Map.Contains(Key))
			{
				Factory<T>::GetInstance()._Map.Emplace(Key, [&]{ return new SubT(Forward<Args>(args)...); });
			}
		}
	};

	static T* Produce(const FName& Key)
	{
		return Factory<T>::GetInstance()._Map[Key]();
	}

	static TSharedPtr<T> Produce_Shared(const FName& Key)
	{
		if (Factory<T>::GetInstance()._Map.Contains(Key))
		{
			return TSharedPtr<T>(Produce(Key));
		}

		return nullptr;
	}

	static TUniquePtr<T> Produce_Unique(const FName& Key)
	{
		if (Factory<T>::GetInstance()._Map.Contains(Key))
		{
			return TUniquePtr<T>(Factory<T>::GetInstance()._Map[Key]());
		}

		return nullptr;
	}

private:
	Factory() = default;
	Factory(const Factory&) = delete;
	Factory(Factory&&) = delete;

	FORCEINLINE static Factory<T>& GetInstance() { static Factory<T> _Instance; return _Instance; }

private:
	TMap<FName, TFunction<T* ()>> _Map;
};


#define REGISTER_ITEM_NAME(Key) reg_for_item_##Key##_
#define REGISTER_BLOCK_NAME(Key) reg_for_block_##Key##_
#define REGISTER_ITEM(T, Key, ...) static Factory<Item>::Register_t<T> REGISTER_ITEM_NAME(Key)(TEXT(#Key), __VA_ARGS__);
#define REGISTER_BLOCK(T, Key, ...) static Factory<Block>::Register_t<T> REGISTER_BLOCK_NAME(Key)(TEXT(#Key), __VA_ARGS__);