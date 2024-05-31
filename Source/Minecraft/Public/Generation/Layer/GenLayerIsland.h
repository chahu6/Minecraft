#pragma once
#include "GenLayer.h"

class GenLayerIsland : public GenLayer
{
public:
	GenLayerIsland(int64 Seed);

public:
	virtual int32* GetInts(int32 AreaX, int32 AreaY, int32 AreaWidth, int32 AreaHeight) override;
};