#pragma once
#include"Vector.h"
#include"Common.h"
#include"CameraMath.h"

namespace 
{
	//1つの面に対して予想される最大頂点数
	static int VERTEX_SIZE = 6;
}

/// <summary>
/// 面を構成する情報を持つ
/// (頂点座標、法線ベクトル)
/// </summary>
struct FaceVertex
{
	FaceVertex() 
	{
		vertexs.reserve(VERTEX_SIZE);
	};

	/// <summary>
	/// 面を構成する情報を保存
	/// </summary>
	/// <param name="_vertex">頂点座標の配列</param>
	/// <param name="_normalVer">法線ベクトル</param>
	FaceVertex(std::vector<Vector3D> _vertex,Vector3D _normalVer) 
	{ 
		//頂点情報を保存
		vertexs.reserve(VERTEX_SIZE);
		vertexs = _vertex;
		vertexs.shrink_to_fit();

		//法線ベクトルを保存
		normalVer = _normalVer;
	}

	~FaceVertex() {};

	/// <summary>
	/// 頂点座標
	/// </summary>
	std::vector<Vector3D> vertexs;

	/// <summary>
	/// 法線ベクトル
	/// </summary>
	Vector3D normalVer;

};

/// <summary>
/// 同次座標を行列で変換する関数
/// </summary>
/// <param name="_facevertex">フェース＜/param＞
/// <param name="_mat">行列</param>
/// <returns>変換後のフェース</returns>
static std::vector<Vector4D> TransformFace(const FaceVertex& _facevertex,const Matrix& _mat)
{
	std::vector<Vector4D> result;

	//頂点情報をトランスフォーム
	result.reserve(VERTEX_SIZE);
	for (auto vertex : _facevertex.vertexs)
	{
		result.push_back(VEC4Transform({ vertex.x,vertex.y,vertex.z,1.0f }, _mat));
	}
	result.shrink_to_fit();

	return result;
}