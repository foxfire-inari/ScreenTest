#pragma once
#include "DxLib.h"
#include "CameraMath.h"
#include "MeshObject.h"
#include <assert.h>

namespace
{
	//アウトコード（ビット演算を行うための物）
	static const int INSIDE = 0;	//000000
	static const int LEFT = 1;	//000001
	static const int RIGHT = 2;	//000010
	static const int BOTTOM = 4;	//000100
	static const int TOP = 8;	//001000
	static const int OUTCODE_NEAR = 16;	//010000
	static const int OUTCODE_FAR = 32;	//100000

	//floatの誤差により無限ループが発生してしまうのを防ぐため
	static const int MAX_ITERATIONS = 10;

	/// <summary>
	/// クリップ座標になった時、どの領域にあるかを閉めるアウトコード
	/// </summary>
	/// <param name="p">点P</param>
	/// <returns>点Pが存在する空間</returns>
	int ComputeOutCode(const Vector4D& p)
	{
		//内側として初期化
		int code = INSIDE;

		//Wが０より小さい時はｐが手前にあることだけが確定している
		//X,Yはクリッピングした後にもう一度回す
		if (p.w < 0.0f)
		{
			code |= OUTCODE_NEAR;
			return code;
		}

		// 上下左右表裏で比較し、外側にあったら対応するビットを立てる

		//同時に起こりえないのでelseでまとめる
		if (p.x < -p.w) { code |= LEFT; }
		else if (p.x > p.w) { code |= RIGHT; }

		if (p.y < -p.w) { code |= BOTTOM; }
		else if (p.y > p.w) { code |= TOP; }

		if (p.z < 0.0f) { code |= OUTCODE_NEAR; }
		else if (p.z > p.w) { code |= OUTCODE_FAR; }

		return code;
	}

	/// <summary>
	/// ０除算の危険がないかチェックする
	/// </summary>
	/// <param name="_denominator">チェックする値</param>
	/// <returns>危険があるか</returns>
	bool DenominatorChack(float _denominator)
	{
		return std::fabsf(_denominator) < NEAR_ZERO;
	}

	/// <summary>
	/// Cohen-Sutherlandアルゴリズムを使用したクリッピング関数
	/// </summary>
	/// <param name="startClip">線形変換時の始点</param>
	/// <param name="endClip">線形変換時の終点</param>
	/// <returns>描画されるか</returns>
	bool ClipLineCohenSutherland(Vector4D& startClip, Vector4D& endClip)
	{
		//始点終点のアウトコードをセット
		int startOutcode = ComputeOutCode(startClip);
		int endOutcode = ComputeOutCode(endClip);


		//現在の反復回数
		int iterations = 0;

		while (iterations < MAX_ITERATIONS)
		{
			iterations++;

			//ビットorをして0なら両端が内側にある
			if ((startOutcode | endOutcode) == 0)
			{
				//線分全体を表示
				return true;
			}
			//両端点が同じ外部領域（右側や下側等）なら表示されない
			else if ((startOutcode & endOutcode) != 0)
			{
				return false;
			}
			//クリッピングが必要な場合
			else
			{
				//外側にある方の点のアウトコードを選択
				int outsideCode = (startOutcode != 0) ? startOutcode : endOutcode;

				//矩形との交点が直線startClipendClipのどの範囲にあるか
				//（0.0f<t<1.0f）の場合は線分startClipendClipのどこか
				float t = 0.0f;

				//交点の座標
				Vector4D intersection;

				//線分の方向ベクトル
				Vector4D direction = {
					endClip.x - startClip.x,
					endClip.y - startClip.y,
					endClip.z - startClip.z,
					endClip.w - startClip.w
				};

				//割り算の分母
				float denominator;

				//outsideCodeに対する境界平面（描画される範囲の境界）との交差tを計算
				if (outsideCode & LEFT)
				{
					// -w<=xを組み替えてx+w<=0にして考える
					denominator = direction.x + direction.w;
					//値が小さい =　" x = -w "にほぼ平行になり、０除算の危険
					if (DenominatorChack(denominator)) { return false; }
					//交点の範囲を計算
					t = (-startClip.x - startClip.w) / denominator;
				}
				else if (outsideCode & RIGHT)
				{
					// x<=w から x-w<=0
					denominator = direction.x - direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (startClip.w - startClip.x) / denominator;
				}
				else if (outsideCode & BOTTOM)
				{
					// -w<=y から y+w<=0
					denominator = direction.y + direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (-startClip.y - startClip.w) / denominator;
				}
				else if (outsideCode & TOP)
				{
					// y<=w から y-w<=0
					denominator = direction.y - direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (startClip.w - startClip.y) / denominator;
				}
				else if (outsideCode & OUTCODE_NEAR)
				{
					//ここだけ z<=0 なので処理がわずかに違う
					if (DenominatorChack(direction.z)) { return false; }
					t = -startClip.z / direction.z;
				}
				else if (outsideCode & OUTCODE_FAR)
				{
					// z<=w から z-w<=0
					denominator = direction.z - direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (startClip.w - startClip.z) / denominator;
				}
				//ここまで来たら何らかのバグが起きていることになる
				else { assert(false); }

				//tが線分上の範囲にない場合は計算をしない
				if (t < 0.0f || t>1.0f) { return false; }

				//交点の座標を計算
				intersection = VectorLerp4D(startClip, endClip, t);

				//外部の点を交点に置き換え、その点のoutcodeを計算しなおす
				if (outsideCode == startOutcode)
				{
					startClip = intersection;
					startOutcode = ComputeOutCode(startClip);
				}
				else
				{
					endClip = intersection;
					endOutcode = ComputeOutCode(endClip);
				}
			}
		}
		//最大まで回るのはfloatが悪さした時
		return false;

	}
}