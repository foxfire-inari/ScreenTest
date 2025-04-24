#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"

_inline Vector3D VECTransform(const Vector3D& _vec, const Matrix& _mat)
{
	Vector3D result;
	result.x = _vec.x * _mat.m[0][0] + _vec.y * _mat.m[1][0] + _vec.z * _mat.m[2][0];
	result.y = _vec.x * _mat.m[0][1] + _vec.y * _mat.m[1][1] + _vec.z * _mat.m[2][1];
	result.z = _vec.x * _mat.m[0][2] + _vec.y * _mat.m[1][2] + _vec.z * _mat.m[2][2];
	return result;
}

/// <summary>
/// �J��������]�ړ�������֐�
/// </summary>
/// <param name="_location">���W�x�N�g��</param>
/// <param name="_rotation">��]���</param>
/// <returns>��]�ړ���̍��W</returns>
static Vector3D CameraTransform(Vector3D& _location, Vector3D& _rotation)
{
	//�p�x�����]�s����쐬
	Matrix rotV = GetMatrixAxisX(_rotation.x);
	Matrix rotH = GetMatrixAxisY(_rotation.y);

	//�v�Z���ʕۑ��p
	Vector3D result = _location;

	//��]�s��ɂ����`�ϊ�
	result = VECTransform(result, rotV);
	result = VECTransform(result, rotH);

	return result;
}
