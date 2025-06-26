#pragma once
#include "DxLib.h"
#include "CameraMath.h"
#include "FaceObject.h"
#include "Quaternion.h"
#include <assert.h>

namespace
{
	/// <summary>
	/// はみ出ている方向
	/// </summary>
	enum OutVec
	{
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,
		OUTVEC_FAR,
		OUTVEC_NEAR
	};

	/// <summary>
	/// ０除算の危険がないかチェックする
	/// </summary>
	/// <param name="_denominator">チェックする値</param>
	/// <returns>危険があるか</returns>
	bool DenominatorChack(float _denominator)
	{
		return std::fabsf(_denominator) < NEAR_ZERO;
	}

	Vector4D intersect(const Vector4D& startClip, const Vector4D& endClip, OutVec outvec)
	{
		//クリップ後の座標
		Vector4D result;

		//矩形との交点が直線startClipendClipのどの範囲にあるか
		//（0.0f<t<1.0f）の場合は線分startClipendClipのどこか
		float t = 0.0f;

		//線分の方向ベクトル
		Vector4D direction = {
			endClip.x - startClip.x,
			endClip.y - startClip.y,
			endClip.z - startClip.z,
			endClip.w - startClip.w
		};
		//割り算の分母
		float denominator;

		//判定面に対する境界平面（描画される範囲の境界）との交差tを計算
		switch (outvec)
		{
		case LEFT:
			// -w<=xを組み替えてx+w<=0にして考える
			denominator = direction.x + direction.w;
			//値が小さい =　" x = -w "にほぼ平行になり、０除算の危険
			if (DenominatorChack(denominator)) { return false; }
			//交点の範囲を計算
			t = (-startClip.x - startClip.w) / denominator;
			break;
		case RIGHT:
			// x<=w から x-w<=0
			denominator = direction.x - direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (startClip.w - startClip.x) / denominator;
			break;
		case BOTTOM:
			// -w<=y から y+w<=0
			denominator = direction.y + direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (-startClip.y - startClip.w) / denominator;
			break;
		case TOP:
			// y<=w から y-w<=0
			denominator = direction.y - direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (startClip.w - startClip.y) / denominator;
			break;
		case OUTVEC_NEAR:
			//ここだけ0z<=0なので処理がわずかに違う
			if (DenominatorChack(direction.z)) { return false; }
			t = -startClip.z / direction.z;
			break;
		case OUTVEC_FAR:
			// z<=w から z-w<=0
			denominator = direction.z - direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (startClip.w - startClip.z) / denominator;
			break;
		default:
			//ここまで来たら何らかのバグが起きていることになる
			assert(false);
		}

		//tが線分上の範囲にない場合は計算をしない
		if (t < 0.0f || t>1.0f) { return false; }

		//交点の座標を計算
		result = VectorLerp4D(startClip, endClip, t);

		return result;
	}

	/// <summary>
	/// 内側にあるか
	/// </summary>
	/// <param name="p">頂点</param>
	/// <param name="outvec">確認する面</param>
	/// <returns>はみ出ているかどうか</returns>
	bool inside(const Vector4D& p, OutVec outvec)
	{
		switch (outvec)
		{
		case LEFT:			return p.x > -p.w;
		case RIGHT:			return p.x < p.w;
		case BOTTOM:		return p.y > -p.w;
		case TOP:			return p.y < p.w;
		case OUTVEC_NEAR:	return p.z > -p.w;
		case OUTVEC_FAR:	return p.z < p.w;
		}
		return false;
	}


	/// <summary>
	/// フェースをクリップし、新しいフェースを作る
	/// </summary>
	/// <param name="face">フェース</param>
	/// <param name="outvec">対象とするクリップ面</param>
	/// <returns>クリップ後のフェース</returns>
	std::vector<Vector4D> clipEdge(const std::vector<Vector4D>& face, OutVec outvec)
	{
		std::vector<Vector4D> result;

		//フェースの辺を一つずつ計算
		for (int i = 0; i < face.size(); ++i)
		{
			Vector4D startvertex = face[i];
			Vector4D endvertex = face[(i + 1) % face.size()];

			bool startIn = inside(startvertex, outvec);
			bool endIn = inside(endvertex, outvec);

			if (startIn && endIn)
			{
				result.push_back(endvertex);
			}
			else if (startIn && !endIn)
			{
				result.push_back(intersect(startvertex, endvertex, outvec));
			}
			else if (!startIn && endIn)
			{
				result.push_back(intersect(startvertex, endvertex, outvec));
				result.push_back(endvertex);
			}
		}
		return result;
	}

	/// <summary>
	/// フェースをクリップ範囲に収める処理
	/// </summary>
	/// <param name="face">フェース</param>
	/// <returns>クリップ後のフェース</returns>
	std::vector<Vector4D> sutherlandHodgmanClip(const std::vector<Vector4D>& face)
	{
		std::vector<Vector4D> result = face;

		//各クリップ面で線分を処理
		for (OutVec e : {LEFT, RIGHT, BOTTOM, TOP, OUTVEC_FAR, OUTVEC_NEAR})
		{
			if (result.size() == 0)return result;
			result = clipEdge(result, e);
		}
		return result;
	}

	/// <summary>
	/// パースペクティブ除算
	/// </summary>
	/// <param name="face">クリップ済みのVector4D配列</param>
	/// <returns>NDC座標の配列</returns>
	FaceVertex ChangeNDC(std::vector<Vector4D> face)
	{
		FaceVertex result;

		Vector3D vec;

		for (int i = 0; i < face.size(); i++)
		{
			vec = { face[i].x / face[i].w,face[i].y / face[i].w,face[i].z / face[i].w };
			result.vertexs.push_back(vec);
		}
		return result;
	}

	/// <summary>
	/// フェースを三角形に分ける
	/// </summary>
	/// <param name="face">クリップされたフェース</param>
	/// <returns>三角形のフェース配列</returns>
	std::vector<FaceVertex> Triangulate(const FaceVertex& face)
	{
		std::vector<FaceVertex> result;

		if (face.vertexs.size() < 3)return result;

		for (int i = 1; i + 1 < face.vertexs.size(); ++i)
		{
			FaceVertex triangle = { { face.vertexs[0],face.vertexs[i],face.vertexs[i + 1] },face.normalVer };
			result.push_back(triangle);
		}
		return result;
	}
}