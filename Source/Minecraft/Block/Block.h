#pragma once

#include "CoreMinimal.h"
#include "BlockState.h"

class AMCPlayer;

class FItem;

struct FBlockPos;

class MINECRAFT_API FBlock : public TSharedFromThis<FBlock>
{
public:
	static TMap<int32, TSharedPtr<FBlock>> BlockRegister;

public:
	struct Properties
	{
		friend class FBlock;

		Properties& Air()
		{
			this->bIsAir = true;
			return *this;
		}

		Properties& OnDestroyTime(float Time)
		{
			this->DestroyTime = Time;
			return *this;
		}

	private:
		float DestroyTime;
		bool bIsAir;
	};

public:
	FBlock(const Properties& properties);

	static TSharedPtr<FBlock> RegisterBlock(int32 BlockID, TSharedPtr<FBlock> NewBlock);
	static TSharedPtr<FBlock> GetBlock(int32 BlockID);

public:
	float GetPlayerRelativeBlockHardness(const AMCPlayer* Player);

	void DropBlockAsItem(UWorld* World, const FBlockPos& BlockPos, int32 BlockID) const;

	TSharedPtr<FItem> GetItemDropped(int32 BlockID) const;

	virtual bool CanDroppedItem() const { return true; }

	virtual void Destroyed() const {}

private:
	bool bIsAir;
	float Hardness;

public:
	FORCEINLINE bool IsAir() const { return bIsAir; }
	FORCEINLINE float GetBlockHardness() const { return Hardness; }

};