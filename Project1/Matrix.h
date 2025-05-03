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

/// <summary>
/// 透視投影行列を作成
/// （左手座標系）
/// </summary>
/// <param name="viewVer">垂直方向の視野角（ラジアン）</param>
/// <param name="aspect">アスペクト比（幅/高さ）</param>
/// <param name="near">最前面までの距離</param>
/// <param name="far">最奥面までの距離</param>
/// <returns>透視投影行列</returns>
static Matrix Perspective(float viewVer, float aspect, float near, float far)
{
	//アス比、最前面までの距離が適切な数値か、最奥面が最前面より手前に来ていないか
	assert(aspect <= 0.0f || near <= 0.0f || far <= near);

	//新しく行列を作る
	Matrix result;
	//Y方向の拡大
	float yScale = 1.0f / tanf(viewVer / 2.0f);
	//X方向の拡大
	float xScale = yScale / aspect;
	//Z方向の係数
	float zRange = (far + near) / (far - near);
	//Z座標の変換係数
	float zOffset = -2 * far * near * (zRange / (far + near));

	//行列の要素をセット
	result.m[0][0] = xScale;	//
	result.m[1][1] = yScale;	//
	result.m[2][2] = zRange;	//Z値を[0,1]の範囲に近づける
	result.m[2][3] = 1.0f;		//WにZ値をコピーするため
	result.m[3][2] = zOffset;	//奥行きに関わる計算

	return result;
}