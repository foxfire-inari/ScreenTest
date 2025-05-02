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

	//オブジェクト生成時に値を初期化
	Matrix() 
	{
		for (int i = 0; i < 4; ++i) 
		{
			for (int j = 0; j < 4; ++j) 
			{
				m[i][j] = 0.0f;
			}
		}
	}

	/// <summary>
	/// 単位行列を返す
	/// </summary>
	/// <returns>単位行列</returns>
	static Matrix Identity()
	{
		Matrix result;
		result.m[0][0] = 1.0f;
		result.m[1][1] = 1.0f;
		result.m[2][2] = 1.0f;
		result.m[3][3] = 1.0f;
		return result;
	}

};

/// <summary>
/// X軸の回転行列を生成する関数
/// （左手座標系）
/// </summary>
/// <param name="XAxisRotation">角度（ラジアン）</param>
/// <returns>X軸回転行列</returns>
static Matrix GetMatrixAxisX(float XAxisRotation)
{
	float sin = sinf(XAxisRotation);
	float cos = cosf(XAxisRotation);

	//単位行列を作成
	Matrix result = Matrix::Identity();
	//回転行列の要素を設定
	result.m[1][1] =  cos; result.m[1][2] = sin;
	result.m[2][1] = -sin; result.m[2][2] = cos;

	//{ 1.0f, 0.0f, 0.0f, 0.0f }
	//{ 0.0f,  cos,  sin, 0.0f }
	//{ 0.0f, -sin,  cos, 0.0f }
	//{ 0.0f, 0.0f, 0.0f, 1.0f }

	return result;
}

/// <summary>
/// Y軸の回転行列を生成する関数
/// （左手座標系）
/// </summary>
/// <param name="YAxisRotation">角度（ラジアン）</param>
/// <returns>Y軸回転行列</returns>
static Matrix GetMatrixAxisY(float YAxisRotation)
{
	float sin = sinf(YAxisRotation);
	float cos = cosf(YAxisRotation);

	//単位行列を作成
	Matrix result = Matrix::Identity();
	//回転行列の要素を設定
	result.m[0][0] =  cos; result.m[0][2] = sin;
	result.m[2][0] = -sin; result.m[2][2] = cos;

	// {  cos, 0.0f, -sin, 0.0f}
	// { 0.0f, 1.0f, 0.0f, 0.0f}
	// {  sin, 0.0f,  cos, 0.0f}
	// { 0.0f, 0.0f, 0.0f, 1.0f}

	return result;
}

/// <summary>
/// Z軸の回転行列を再生する関数
/// （左手座標系）
/// </summary>
/// <param name="ZAxisRotation">角度（ラジアン）</param>
/// <returns>Z軸回転行列</returns>
static Matrix GetMatrixAxisZ(float ZAxisRotation)
{
	float sin = sinf(ZAxisRotation);
	float cos = cosf(ZAxisRotation);

	//単位行列を作成
	Matrix result = Matrix::Identity();
	//回転行列の要素を設定
	result.m[0][0] = cos; result.m[0][1] = -sin;
	result.m[1][0] = sin; result.m[1][1] = cos;

	return result;
}

/// <summary>
/// 行列の乗算
/// </summary>
/// <param name="second">後に計算される行列</param>
/// <param name="first">先に計算される行列</param>
/// <returns>行列の積</returns>
static Matrix MartixMultiply(const Matrix& second, const Matrix& first)
{
	Matrix result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += second.m[i][k] * first.m[k][j];
			}
		}
	}
	return result;
}

//------------------ここから開始する