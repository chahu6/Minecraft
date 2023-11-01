// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplexNoiseLibrary.h"

#define FASTFLOOR(x) (((x) > 0) ? ((int)x) : (((int)x) - 1))

unsigned char USimplexNoiseLibrary::perm[512] = { 
	151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

	151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

float USimplexNoiseLibrary::_grad(int hash, float x)
{
	int h = hash & 15;
	float grad = 1.0 + (h & 7);							// Gradient value 1.0, 2.0, ..., 8.0
	if (h & 8) grad = -grad;							// Set a random sign for the gradient
	return (grad * x);									// Multiply the gradient with the distance
}

// 梯度函数
// 使用了位翻转(bit-flipping)技巧来实现
float USimplexNoiseLibrary::_grad(int hash, float x, float y)
{
	int h = hash & 7;
	float u = h < 4 ? x : y;
	float v = h < 4 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // 梯度向量的长度是根号5；
}

// 返回值 [-1, 1];应该比这个范围大点应该有一点多
float USimplexNoiseLibrary::_simplexNoise2D(float x, float y)
{
#define F2 0.366025403f							// F2 = 0.5*(sqrt(3.0)-1.0)
#define G2 0.211324865f							// G2 = (3.0-Math.sqrt(3.0))/6.0

	float n0, n1, n2;

	float s = (x + y) * F2;		// 笛卡尔坐标系的点变换到正三角形坐标系的点
	float xs = x + s;
	float ys = y + s;
	int i = FASTFLOOR(xs);		// 正三角形坐标系的左下角的坐标
	int j = FASTFLOOR(ys);		

	float t = (float)(i + j) * G2;	// 正三角形坐标系的点变换到笛卡尔坐标系的点
	float X0 = i - t;
	float Y0 = j - t;

	float x0 = x - X0;		// 向左下角的点指向点P的方向向量
	float y0 = y - Y0;

	// 确定单形
	int i1, j1;		// 用于存储点P实在上三角形（0，1），还是下三角形（1，0）
	if (x0 > y0)	// 下三角
	{
		i1 = 1;
		j1 = 0;
	}
	else            // 上三角形
	{
		i1 = 0;
		j1 = 1;
	}

	/*
	最新：可以这么想（X0 + 1, Y0）点，加上（G2， G2）向量，
	就直接到了变换后的正三角形的那个顶点了，在（x, y）减去他（X0 + 1 + G2, Y0 + G2），就是
	答案了
	*/ 
	// 也可以这么像（x0, y0） - (i1, j1)就是另一个点指向点P的方向向量
	float x1 = x0 - i1 + G2;		// 直接算的是方向向量，其实是化简的表示，例如是先三角形时：x - (X0 + 1 + G2)，y = (Y0 + G2);其实就是笛卡尔坐标系下的右下角点加上变换的原本的点到变换的点的向量在被减去。（x, y）、（X0 + 1, Y0）、(G2, G2)，应该吧
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0f + 2.0f * G2;
	float y2 = y0 - 1.0f + 2.0f * G2;

	// 防止超出255
	int ii = i & 0xFF; // 八位
	int jj = j & 0xFF;

	// 计算权重，衰减函数max(0.0f, r^2 - (x^2 + y^2))^4 * _grad; _grad是梯度函数
	float t0 = 0.5f - x0 * x0 - y0 * y0;
	if (t0 < 0.0f) n0 = 0.0f;
	else
	{
		t0 *= t0;
		n0 = t0 * t0 * _grad(perm[ii + perm[jj]], x0, y0); // 梯度值选择
	}

	float t1 = 0.5f - x1 * x1 - y1 * y1;
	if (t1 < 0.0f) n1 = 0.0f;
	else
	{
		t1 *= t1;
		n1 = t1 * t1 * _grad(perm[ii + i1 + perm[jj + j1]], x1, y1);
	}

	float t2 = 0.5f - x2 * x2 - y2 * y2;
	if (t2 < 0.0f) n2 = 0.0f;
	else
	{
		t2 *= t2;
		n2 = t2 * t2 * _grad(perm[ii + 1 + perm[jj + 1]], x2, y2);
	}

	return 40.0f / 0.884343445f * (n0 + n1 + n2); // 最大值应该是梯度向量的长度不要忘记考虑进去，44.36552715791845，所以应该乘以45，但是不知道他这个0.884343445哪来的，但是结果还是40.0f / 0.884343445f = 45.23129585700723跟45差不多就这样吧
}

void USimplexNoiseLibrary::SetNoiseSeed(const int32 newSeed)
{
	FMath::RandInit(newSeed);
	for (uint16 it = 0; it < 256; ++it)
	{
		uint8 nextNum = FMath::RandRange(0, 255);
		USimplexNoiseLibrary::perm[it] = (unsigned char)nextNum;
		USimplexNoiseLibrary::perm[it + 256] = (unsigned char)nextNum;
	}
}

float USimplexNoiseLibrary::SimplexNoise2D(float x, float y, float inFactor)
{
	return _simplexNoise2D(x * inFactor, y * inFactor);
}

float USimplexNoiseLibrary::SimplexNoiseInRange2D(float x, float y, float rangeMin, float rangeMax, float inFactor)
{
	if (rangeMax < rangeMin) rangeMax = rangeMin + 1.0f;
	float nval = (SimplexNoise2D(x, y, inFactor) + 1) * 0.5f;
	return nval * (rangeMax - rangeMin) + rangeMin;
}
