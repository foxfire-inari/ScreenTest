#pragma once
#include <math.h>
#include <assert.h>

//インライン関数：短い関数を通常より早く実行できる。

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
	/// 自身を正規化
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

	/// <summary>
	/// 自身を正規化したときのベクトルを生成して返す
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector3D Normalized() const
	{
		//自分のコピーを作成
		Vector3D result = *this;
		//コピーを正規化
		result.Normalize();
		//正規化されたコピーを返す
		return result;
	}

};

/// <summary>
/// ベクトルの内積を求める
/// </summary>
/// <param name="aVec">同じ始点を持つベクトルa</param>
/// <param name="bVec">同じ始点を持つベクトルb</param>
/// <returns>内積</returns>
inline float Dot(const Vector3D aVec, const Vector3D bVec)
{
	return aVec.x*bVec.x + aVec.y*bVec.y + aVec.z*bVec.z;
}

/// <summary>
/// ベクトルの外積を求める
/// </summary>
/// <param name="rightVec">右辺にあたるベクトル</param>
/// <param name="leftVec">左辺にあたるベクトル</param>
/// <returns>外積</returns>
static Vector3D Cross(const Vector3D rightVec, const Vector3D leftVec)
{
	Vector3D result;
	result.x = rightVec.y*leftVec.z - rightVec.z*leftVec.y;
	result.y = rightVec.z*leftVec.x - rightVec.x*leftVec.z;
	result.z = rightVec.x*leftVec.y - rightVec.y*leftVec.x;
	return result;
}

static float AngleOfVector(const Vector3D aVec, const Vector3D bVec)
{
	Vector3D aLength = aVec.Normalized();
	Vector3D bLength = bVec.Normalized();
	assert

}