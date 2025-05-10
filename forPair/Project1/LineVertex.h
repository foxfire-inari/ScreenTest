#pragma once
#include <utility>
#include"Vector.h"

/// <summary>
/// ü•ª‚ğ\¬‚·‚é“ñ‚Â‚Ì’¸“_‚ğ‚Â
/// </summary>
struct LineVertex
{
	LineVertex() {}
	LineVertex(Vector3D _first, Vector3D _second) { vertexPair = std::make_pair(_first,_second); }
	
	~LineVertex() {};

	//ü•ª‚Ì’¸“_
	std::pair<Vector3D, Vector3D> vertexPair;
};