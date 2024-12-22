// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Gen/WorldGenTrees.h"
#include "Math/BlockPos.h"
#include "World/WorldManager.h"
#include "Init/Blocks.h"
#include "World/WorldGenerator.h"

bool UWorldGenTrees::Generate(AWorldManager* InWorldManager, const FBlockPos& Position) const
{
	const int32 TreeHeight = FMath::RandHelper(3) + MinTreeHeight;
	bool bFlag = true;

	if (Position.Z >= 1 && Position.Z + TreeHeight + 1 <= 256)
	{
		for (int32 Height = Position.Z; Height <= Position.Z + TreeHeight + 1; ++Height)
		{
			int32 k = 0;
			if (Height == Position.Z)
			{
				k = 0;
			}

			if (Height >= Position.Z + 1 + TreeHeight - 2)
			{
				k = 2;
			}

			for (int32 X = Position.X - k; X <= Position.X + k; ++X)
			{
				for (int32 Y = Position.Y - k; Y <= Position.Y + k; ++Y)
				{
					if (Height >= 0 && Height < WorldGenerator::CHUNK_HEIGHT)
					{
						if (!CanGrowInto(InWorldManager->GetBlockState(X, Y, Height).GetBlock()))
						{
							bFlag = false;
						}
					}
					else
					{
						bFlag = false;
					}
				}
			}
		}

		if (!bFlag)
		{
			return false;
		}
		else
		{
			const UBlock* DownBlock = InWorldManager->GetBlockState(Position.Down()).GetBlock();
			if (DownBlock == UBlocks::Grass || DownBlock == UBlocks::Dirt || Position.Z < WorldGenerator::CHUNK_HEIGHT - TreeHeight - 1)
			{
				// 生成树叶
				for (int32 Z = Position.Z - 3 + TreeHeight; Z <= Position.Z + TreeHeight; ++Z)
				{
					const int32 LeafHeight = Z - (Position.Z + TreeHeight);
					const int32 LeafRangage = 1 - LeafHeight / 2;

					for (int32 X = Position.X - LeafRangage; X <= Position.X + LeafRangage; ++X)
					{
						const int32 StartX = X - Position.X;

						for (int32 Y = Position.Y - LeafRangage; Y <= Position.Y + LeafRangage; ++Y)
						{
							const int32 StartY = Y - Position.Y;

							if (FMath::Abs(StartX) != LeafRangage || FMath::Abs(StartY) != LeafRangage || FMath::RandHelper(2) != 0 || LeafHeight != 0)
							{
								const UBlock* Block = InWorldManager->GetBlockState(X, Y, Z).GetBlock();

								// 这种判断也行，就是太长，不好看
								//if (Block->Tag.MatchesAnyExact(FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{ FMinecraftGameplayTags::Get().Air, FMinecraftGameplayTags::Get().Item_Block_Leaves })))
								if (Block == UBlocks::Air || Block == UBlocks::Leaves)
								{
									InWorldManager->SetBlockState(X, Y, Z, UBlocks::Leaves->GetDefaultBlockState());
								}
							}
						}
					}
				}

				// 生成树干
				for (int32 Z = 0; Z < TreeHeight; ++Z)
				{
					const UBlock* Block = InWorldManager->GetBlockState(Position.Up(Z)).GetBlock();

					if (Block == UBlocks::Air || Block == UBlocks::Leaves)
					{
						InWorldManager->SetBlockState(Position.Up(Z), UBlocks::Log->GetDefaultBlockState());
					}
				}
			}
		}
	}

	return false;
}
