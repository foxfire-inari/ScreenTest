#pragma once
#include <math.h>

/// <summary>
/// 三次元の正方行列
/// wまであるのは
/// ベクトルの平行移動変換で使うから
/// </summary>
struct Matrix
{
	float m[4][4];
};

// X軸回転行列
static Matrix GetMatrixAxisX(float XAxisRotation)
{
	float sin, cos;
	sin = sinf(XAxisRotation);
	cos = cosf(XAxisRotation);

	Matrix result =
	{
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f},
			{ 0.0f,  cos,  sin, 0.0f},
			{ 0.0f, -sin,  cos, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	return result;
}

// Y軸回転行列
static Matrix GetMatrixAxisY(float YAxisRotation)
{
	float sin, cos;
	sin = sinf(YAxisRotation);
	cos = cosf(YAxisRotation);

	Matrix result =
	{
		{
			{  cos, 0.0f, -sin, 0.0f},
			{ 0.0f, 1.0f, 0.0f, 0.0f},
			{  sin, 0.0f,  cos, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	return result;
}