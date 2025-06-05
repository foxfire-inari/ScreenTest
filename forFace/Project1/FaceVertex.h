#pragma once
#include"Vector.h"
#include"Common.h"

namespace 
{
	//1�̖ʂɑ΂��ė\�z�����ő咸�_��
	static int VERTEX_SIZE = 6;
}

/// <summary>
/// �ʂ��\�������������
/// (���_���W�A�@���x�N�g��)
/// </summary>
struct FaceVertex
{
	FaceVertex() 
	{
		vertexs.reserve(VERTEX_SIZE);
	};
	/// <summary>
	/// �ʂ��\���������ۑ�
	/// </summary>
	/// <param name="_vertex">���_���W�̔z��</param>
	/// <param name="_normalVer">�@���x�N�g��</param>
	FaceVertex(std::vector<Vector3D> _vertex,Vector3D _normalVer) 
	{ 
		//���_����ۑ�
		vertexs.reserve(VERTEX_SIZE);
		vertexs = _vertex;
		vertexs.shrink_to_fit();

		//�@���x�N�g����ۑ�
		normalVer = _normalVer;
	}

	~FaceVertex() {};

	/// <summary>
	/// ���_���W
	/// </summary>
	std::vector<Vector3D> vertexs;

	/// <summary>
	/// �@���x�N�g��
	/// </summary>
	Vector3D normalVer;

};

