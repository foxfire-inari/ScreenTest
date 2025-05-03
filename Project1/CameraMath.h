#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"

/// <summary>
/// 透視投影等の非線形な計算をするのに必要な座標
/// </summary>
struct Vector4D
{
	float x, y, z, w;
	Vector4D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
		: x(_x), y(_y), z(_z), w(_w) {}
};

/// <summary>
/// 3Dベクトルを行列で変換する関数
/// （平行移動は行われない）
/// </summary>
/// <param name="_vec">3Dベクトル</param>
/// <param name="_mat">行列</param>
/// <returns>変換後のベクトル</returns>
inline Vector3D VEC3Transform(const Vector3D& _vec, const Matrix& _mat)
{
	Vector3D result;
	result.x = _vec.x * _mat.m[0][0] + _vec.y * _mat.m[1][0] + _vec.z * _mat.m[2][0];
	result.y = _vec.x * _mat.m[0][1] + _vec.y * _mat.m[1][1] + _vec.z * _mat.m[2][1];
	result.z = _vec.x * _mat.m[0][2] + _vec.y * _mat.m[1][2] + _vec.z * _mat.m[2][2];
	return result;
}

/// <summary>
/// 同次座標を行列で変換する関数
/// </summary>
/// <param name="_vec">4Dベクトル</param>
/// <param name="_mat">行列</param>
/// <returns>変換後のベクトル</returns>
inline Vector4D VEC4Transform(const Vector4D& _vec, const Matrix& _mat)
{
    Vector4D result;
    result.x = _vec.x * _mat.m[0][0] + _vec.y * _mat.m[1][0] + _vec.z * _mat.m[2][0] + _vec.w * _mat.m[3][0];
    result.y = _vec.x * _mat.m[0][1] + _vec.y * _mat.m[1][1] + _vec.z * _mat.m[2][1] + _vec.w * _mat.m[3][1];
    result.z = _vec.x * _mat.m[0][2] + _vec.y * _mat.m[1][2] + _vec.z * _mat.m[2][2] + _vec.w * _mat.m[3][2];
    result.w = _vec.x * _mat.m[0][3] + _vec.y * _mat.m[1][3] + _vec.z * _mat.m[2][3] + _vec.w * _mat.m[3][3];

    return result;
}