#pragma once
#include"Common.h"
#include"FaceVertex.h"

/// <summary>
/// �ʏ����I�u�W�F�N�g�P�ʂŎ��\����
/// </summary>
struct MeshObject
{
	MeshObject() {}
	//��ɗv�f���m�ۂ���i������x�̗v�f�����K�v�Ȏ��j
	MeshObject(int _element) { faceVertexs.reserve(_element); }
	~MeshObject() {}

	//�I�u�W�F�N�g���\������S�Ă̐���
	std::vector<FaceVertex> faceVertexs;
};

