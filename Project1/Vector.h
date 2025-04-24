#pragma once
#include <math.h>

/// <summary>
/// 三次元ベクトル
/// </summary>
struct Vector3D
{
	float x, y, z;

	//加算(+)
	Vector3D operator +(Vector3D _vec)
	{
		Vector3D vec;
		vec.x = this->x + _vec.x;
		vec.y = this->y + _vec.y;
		vec.z = this->z + _vec.z;
		return vec;
	}

	//加算(+=)
	void operator +=(Vector3D _vec)
	{
		this->x += _vec.x;
		this->y += _vec.y;
		this->z += _vec.z;
	}

	//減算(-)
	Vector3D operator -(Vector3D _vec)
	{
		Vector3D vec;
		vec.x = this->x - _vec.x;
		vec.y = this->y - _vec.y;
		vec.z = this->z - _vec.z;
		return vec;
	}
	//減算(-=)
	void operator -=(Vector3D _vec)
	{
		this->x -= _vec.x;
		this->y -= _vec.y;
		this->z -= _vec.z;
	}

	//float型でスケーリング(*)
	Vector3D operator *(float _scale)
	{
		Vector3D result;
		result.x = this->x * _scale; 
		result.y = this->y * _scale;
		result.z = this->z * _scale;
		return result;
	}
	//float型でスケーリング(*=)
	void operator *=(float _scale)
	{
		this->x *= _scale;
		this->y *= _scale;
		this->z *= _scale;
	}
};

//インライン関数：短い関数を通常より早く実行できる。

/// <summary>
/// ベクトルを作成する
/// </summary>
/// <param name="_x">X座標</param>
/// <param name="_y">Y座標</param>
/// <param name="_z">Z座標</param>
/// <returns>作成されたベクトル</returns>
_inline Vector3D SetVec3D(float _x,float _y,float _z)
{
	Vector3D result;
	result.x = _x;
	result.y = _y;
	result.z = _z;
	return result;
}
