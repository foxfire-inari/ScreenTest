#pragma once
#include"Vector.h"
#include"Common.h"

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

