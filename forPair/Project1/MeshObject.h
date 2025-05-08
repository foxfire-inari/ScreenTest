#pragma once
#include"Common.h"
#include"LineVertex.h"

/// <summary>
/// 線分をオブジェクト単位で持つ構造体
/// </summary>
struct MeshObject
{
	MeshObject() {};
	~MeshObject() {};

	//オブジェクトを構成する全ての線分
	std::vector<LineVertex> lineVertexs;
};

