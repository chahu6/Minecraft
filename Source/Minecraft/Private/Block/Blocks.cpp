#include "Block/Blocks.h"

TMap<int32, TSharedPtr<FBlock>> Blocks::BlocksMap;

TSharedPtr<FBlock> Blocks::AIR = RegisterBlock(0, MakeShared<FBlock>(FBlock::Properties().Air()));
TSharedPtr<FBlock> Blocks::STONE = RegisterBlock(1, MakeShared<FBlock>(FBlock::Properties().OnDestroyTime(2.f)));
TSharedPtr<FBlock> Blocks::DIRT = RegisterBlock(2, MakeShared<FBlock>(FBlock::Properties().OnDestroyTime(0.5f)));
TSharedPtr<FBlock> Blocks::GRASS = RegisterBlock(3, MakeShared<FBlock>(FBlock::Properties().OnDestroyTime(0.6f)));

TSharedPtr<FBlock> Blocks::RegisterBlock(int32 BlockID, TSharedPtr<FBlock> NewBlock)
{
    return FBlock::RegisterBlock(BlockID, NewBlock);
}
