#pragma once
#include"Common.h"
#include"FaceVertex.h"

/// <summary>
/// �ʏ����I�u�W�F�N�g�P�ʂŎ��\����
/// </summary>
struct FaceObject
{
	FaceObject() {}
	//��ɗv�f���m�ۂ���i������x�̗v�f�����K�v�Ȏ��j
	FaceObject(int _element) { faceVertexs.reserve(_element); }
	~FaceObject() {}

	//�I�u�W�F�N�g���\������S�Ă̒��_
	std::vector<FaceVertex> faceVertexs;
};

