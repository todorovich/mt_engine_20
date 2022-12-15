//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
//***************************************************************************************
module;

#include <cstdlib>
#include <DirectXMath.h>

export module MathHelper;

export struct MathHelper
{
	static const float Infinity;
	static const float Pi;

	// Returns random float in [0, 1).
	static float RandF();

	// Returns random float in [a, b).
	static float RandF(float a, float b);

	static int Rand(int a, int b);

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);
};

using namespace DirectX;

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;

// Returns random float in [0, 1).
inline float MathHelper::RandF()
{
	return (float)(rand()) / (float)0x7fff;
}

// Returns random float in [a, b).

inline float MathHelper::RandF(float a, float b)
{
	return a + RandF() * (b - a);
}

inline int MathHelper::Rand(int a, int b)
{
	return a + rand() % ((b - a) + 1);
}

// Returns the polar angle of the point (x,y) in [0, 2*PI).

inline float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + Pi; // in [0, 2*pi).

	return theta;
}