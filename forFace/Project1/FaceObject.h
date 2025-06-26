#pragma once
#include"Common.h"
#include"FaceVertex.h"

/// <summary>
/// 面情報をオブジェクト単位で持つ構造体
/// </summary>
struct FaceObject
{
	FaceObject() {}
	//先に要素を確保する（ある程度の要素数が必要な時）
	FaceObject(int _element) { faceVertexs.reserve(_element); }
	~FaceObject() {}

	//オブジェクトを構成する全ての頂点
	std::vector<FaceVertex> faceVertexs;
};

