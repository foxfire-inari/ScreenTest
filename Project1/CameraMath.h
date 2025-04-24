#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"

_inline Vector3D VECTransform(const Vector3D& _vec, const Matrix& _mat)
{
	Vector3D result;
	result.x = _vec.x * _mat.m[0][0] + _vec.y * _mat.m[1][0] + _vec.z * _mat.m[2][0];
	result.y = _vec.x * _mat.m[0][1] + _vec.y * _mat.m[1][1] + _vec.z * _mat.m[2][1];
	result.z = _vec.x * _mat.m[0][2] + _vec.y * _mat.m[1][2] + _vec.z * _mat.m[2][2];
	return result;
}

/// <summary>
/// カメラを回転移動させる関数
/// </summary>
/// <param name="_location">座標ベクトル</param>
/// <param name="_rotation">回転情報</param>
/// <returns>回転移動後の座標</returns>
static Vector3D CameraTransform(Vector3D& _location, Vector3D& _rotation)
{
	//角度から回転行列を作成
	Matrix rotV = GetMatrixAxisX(_rotation.x);
	Matrix rotH = GetMatrixAxisY(_rotation.y);

	//計算結果保存用
	Vector3D result = _location;

	//回転行列による線形変換
	result = VECTransform(result, rotV);
	result = VECTransform(result, rotH);

	return result;
}
