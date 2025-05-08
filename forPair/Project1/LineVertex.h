#pragma once
#include <utility>
#include"Vector.h"

/// <summary>
/// 線分を構成する二つの頂点を持つ
/// （本当はただ名前付けしたいだけなので、今後変更を加えたい）
/// </summary>
struct LineVertex
{
	LineVertex() { vertexPair = {}; }
	LineVertex(Vector3D _first, Vector3D _second) { vertexPair = {_first,_second}; }
	
	~LineVertex() {};

	//線分の頂点
	std::pair<Vector3D, Vector3D> vertexPair;
};

