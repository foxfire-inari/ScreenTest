#pragma once
#include"Common.h"
#include"LineVertex.h"

/// <summary>
/// �������I�u�W�F�N�g�P�ʂŎ��\����
/// </summary>
struct MeshObject
{
	MeshObject() {};
	~MeshObject() {};

	//�I�u�W�F�N�g���\������S�Ă̐���
	std::vector<LineVertex> lineVertexs;
};

