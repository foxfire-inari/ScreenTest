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

/// <summary>
/// 4Dベクトル start から end へ、パラメータ t (0.0～1.0) を使って線形補間するインライン関数 
///（クリッピングで交点を計算する際に使用する）
/// </summary>
/// <param name="start">開始位置</param>
/// <param name="end">終了位置</param>
/// <param name="t">進行度</param>
/// <returns>4Dベクトル</returns>
inline Vector4D VectorLerp4D(const Vector4D& start, const Vector4D& end, float t)
{
	return {
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t,
		start.w + (end.w - start.w) * t
	};
}