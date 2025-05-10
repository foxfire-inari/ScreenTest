#pragma once
#include"Common.h"
#include"LineVertex.h"

/// <summary>
/// 線分をオブジェクト単位で持つ構造体
/// </summary>
struct MeshObject
{
	MeshObject() {}
	//先に要素を確保する（ある程度の要素数が必要な時）
	MeshObject(int _element) { lineVertexs.reserve(_element); }
	~MeshObject() {}

	//オブジェクトを構成する全ての線分
	std::vector<LineVertex> lineVertexs;
};

