#pragma once
#include <utility>
#include"Vector.h"

/// <summary>
/// �������\�������̒��_������
/// </summary>
struct LineVertex
{
	LineVertex(Vector3D _first, Vector3D _second) { vertexPair = {_first,_second}; };
	~LineVertex() {};

	//�����̒��_
	std::pair<Vector3D, Vector3D> vertexPair;
};

