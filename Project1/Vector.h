#pragma once
#include <math.h>

/// <summary>
/// 三次元ベクトル
/// </summary>
struct Vector3D
{
	float x, y, z;

	//オブジェクト生成時に値を初期化
	Vector3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) 
		:x{ _x },y{ _y },z{ _z }{}

	//加算(+)
	Vector3D operator +(const Vector3D _vec)const { return { x + _vec.x, y + _vec.y, z + _vec.z }; }
	//加算(+=)
	void operator +=(Vector3D _vec) { x += _vec.x; y += _vec.y; z += _vec.z; }

	//減算(-)
	Vector3D operator -(const Vector3D _vec)const { return { x - _vec.x, y - _vec.y, z - _vec.z }; }
	//減算(-=)
	void operator -=(Vector3D _vec) { x -= _vec.x; y -= _vec.y; z -= _vec.z; }

	//float型でスケーリング(*)
	Vector3D operator*(float _scale) const { return { x * _scale, y * _scale, z * _scale }; }
	//float型でスケーリング(*=)
	void operator*=(float _scale) { x *= _scale; y *= _scale; z *= _scale; }


	/// <summary>
	/// 比較用の長さを取得
	///（平方根の計算は負荷が大きいため、長さの比較はこれを使う）
	/// </summary>
	/// <returns>比較用の長さ</returns>
	float Length() { return x*x + y*y + z*z; }

	/// <summary>
	/// ベクトルの長さを取得
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	float LengthSq() { return sqrtf(Length()); }

	/// <summary>
	/// 自信を正規化
	/// </summary>
	void Normalize()
	{
		//自身の長さを取得
		float length = LengthSq();

		//float型が正確に表現できる桁（23ビット≒7桁）
		//非常に小さい値より大きいかを確認してから計算する（０除算をしないため）
		if (length > 1e-12f)
		{
			//除算と乗算では除算の方が負荷が大きいため、逆数を取得しておく
			float invLen = 1.0f / length;

			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
		//非常に小さい値ならゼロベクトルにする
		else { x = 0; y = 0; x = 0; }
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
