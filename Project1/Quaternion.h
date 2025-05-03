#pragma once
#include <cmath>
#include "Vector.h"
#include "Matrix.h"

/*
* オイラー角は順番に回転させる都合上ジンバルロックという状態になる
* Y→X→Zの順番で回転する場合、仮にXを90度回転したとしたら
* YとZの回転方向が同じになってしまう。
* https://www.youtube.com/watch?v=zc8b2Jo7mno
* ↑の動画が一番ジンバルロックを感覚的に理解しやすかった。
*
* クオータニオンは（x,y,z,w）という要素でできている
* XYZは回転の軸、Wは回転の量を表している
* x^2+y^2+z^2+w^2=1
*/

/// <summary>
/// クオータニオン
/// </summary>
struct Quaternion
{
	float x, y, z, w;
	//生成時に単位クオータニオンで初期化できるコンストラクタ
	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	//生成時に各成分をセットして初期化するコンストラクタ
	Quaternion(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w) {}

	/// <summary>
	/// 単位クオータニオンを返す
	/// </summary>
	/// <returns>単位クオータニオン</returns>
	static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

    /// <summary>
    /// axisを軸にangleだけ回転するクオータニオンを生成
    /// </summary>
    /// <param name="axis">軸</param>
    /// <param name="angle">回転量（ラジアン）</param>
    /// <returns>クオータニオン</returns>
    static Quaternion FromAxisAngle(const Vector3D& axis, float angle) 
	{
		//式の都合上回転量を半減させる必要がある
        float halfAngle = angle * 0.5f;
        float sin = sinf(halfAngle);
        float cos = cosf(halfAngle);

		// 回転軸は必ず正規化する
        Vector3D normalizedAxis = axis.Normalized(); 

		//計算結果をクオータニオンとして保存
        Quaternion result(normalizedAxis.x * sin, normalizedAxis.y * sin, normalizedAxis.z * sin, cos);

        return result;
    }

	/// <summary>
	/// クォータニオンの乗算
	/// </summary>
	/// <param name="qua">クォータニオン</param>
	/// <returns>this*qua</returns>
	Quaternion operator*(const Quaternion& qua)const
	{
		Quaternion result;
		result.w = w * qua.w - x * qua.x - y * qua.y - z * qua.z;
		result.x = w * qua.x + x * qua.w + y * qua.z - z * qua.y;
		result.y = w * qua.y - x * qua.z + y * qua.w + z * qua.x;
		result.z = w * qua.z + x * qua.y - y * qua.x + z * qua.w;
		return result;
	}

	/// <summary>
	/// クォータニオンの乗算
	/// </summary>
	/// <param name="qua">クォータニオン</param>
	/// <returns>this*qua</returns>
	Quaternion& operator*=(const Quaternion& qua)
	{
		*this = *this * qua;
		return *this;
	}

	/// <summary>
	/// 比較用の長さを取得（長さの二乗を求める）
	///（平方根の計算は負荷が大きいため、長さの比較はこれを使う）
	/// </summary>
	/// <returns>比較用の長さ</returns>
	float LengthSq()const { return x * x + y * y + z * z + w * w; }

	/// <summary>
	/// クォータニオンの長さを取得
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	float Length()const { return sqrtf(LengthSq()); }

	/// <summary>
	/// 自身を正規化
	/// </summary>
	void Normalize()
	{
		//自身の長さを取得
		float length = Length();

		//float型が正確に表現できる桁（23ビット≒7桁）
		//非常に小さい値より大きいかを確認してから計算する（０除算をしないため）
		if (length > 1e-12f)
		{
			//除算と乗算では除算の方が負荷が大きいため、逆数を取得しておく
			float invLen = 1.0f / length;

			x *= invLen;
			y *= invLen;
			z *= invLen;
			w *= invLen;
		}
		//非常に小さい値なら単位クォータニオンにする
		else { *this = Identity(); }
	}

	/// <summary>
	/// 自身を正規化したときのクォータニオンを生成して返す
	/// </summary>
	/// <returns>正規化されたクォータニオン</returns>
	Quaternion Normalized()const
	{
		//自分のコピーを作成
		Quaternion result = *this;
		//コピーを正規化
		result.Normalize();
		//正規化されたコピーを返す
		return result;
	}

	/// <summary>
	/// 共役クォータニオンを生成して返す
	/// </summary>
	/// <returns>共役クォータニオン</returns>
	Quaternion Conjugate() const { return Quaternion(-x, -y, z, w); }

	//次はここから

};







