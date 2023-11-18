#pragma once

namespace Utils
{
	static bool OutOfBounds(int32 X, int32 Min, int32 Max)
	{
		return X >= Min && X < Max;
	}
}