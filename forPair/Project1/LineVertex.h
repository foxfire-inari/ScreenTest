#pragma once
#include <utility>
#include"Vector.h"

/// <summary>
/// �������\�������̒��_������
/// �i�{���͂������O�t�������������Ȃ̂ŁA����ύX�����������j
/// </summary>
struct LineVertex
{
	LineVertex() { vertexPair = {}; }
	LineVertex(Vector3D _first, Vector3D _second) { vertexPair = {_first,_second}; }
	
	~LineVertex() {};

	//�����̒��_
	std::pair<Vector3D, Vector3D> vertexPair;
};

