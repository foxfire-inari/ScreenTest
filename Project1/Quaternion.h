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
    /// axisをangleだけ回転するクオータニオンを生成
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

	//続きはここから

};







