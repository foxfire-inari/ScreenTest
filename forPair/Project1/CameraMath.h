#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"

/// <summary>
/// �������e���̔���`�Ȍv�Z������̂ɕK�v�ȍ��W
/// </summary>
struct Vector4D
{
	float x, y, z, w;
	Vector4D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
		: x(_x), y(_y), z(_z), w(_w) {}
};

/// <summary>
/// 3D�x�N�g�����s��ŕϊ�����֐�
/// �i���s�ړ��͍s���Ȃ��j
/// </summary>
/// <param name="_vec">3D�x�N�g��</param>
/// <param name="_mat">�s��</param>
/// <returns>�ϊ���̃x�N�g��</returns>
inline Vector3D VEC3Transform(const Vector3D& _vec, const Matrix& _mat)
{
	Vector3D result;
	result.x = _vec.x * _mat.m[0][0] + _vec.y * _mat.m[1][0] + _vec.z * _mat.m[2][0];
	result.y = _vec.x * _mat.m[0][1] + _vec.y * _mat.m[1][1] + _vec.z * _mat.m[2][1];
	result.z = _vec.x * _mat.m[0][2] + _vec.y * _mat.m[1][2] + _vec.z * _mat.m[2][2];
	return result;
}

/// <summary>
/// �������W���s��ŕϊ�����֐�
/// </summary>
/// <param name="_vec">4D�x�N�g��</param>
/// <param name="_mat">�s��</param>
/// <returns>�ϊ���̃x�N�g��</returns>
inline Vector4D VEC4Transform(const Vector4D& _vec, const Matrix& _mat)
{
    Vector4D result;
    result.x = _vec.x * _mat.m[0][0] + _vec.y * _mat.m[1][0] + _vec.z * _mat.m[2][0] + _vec.w * _mat.m[3][0];
    result.y = _vec.x * _mat.m[0][1] + _vec.y * _mat.m[1][1] + _vec.z * _mat.m[2][1] + _vec.w * _mat.m[3][1];
    result.z = _vec.x * _mat.m[0][2] + _vec.y * _mat.m[1][2] + _vec.z * _mat.m[2][2] + _vec.w * _mat.m[3][2];
    result.w = _vec.x * _mat.m[0][3] + _vec.y * _mat.m[1][3] + _vec.z * _mat.m[2][3] + _vec.w * _mat.m[3][3];

    return result;
}

/// <summary>
/// 4D�x�N�g�� start ���� end �ցA�p�����[�^ t (0.0�`1.0) ���g���Đ��`��Ԃ���C�����C���֐� 
///�i�N���b�s���O�Ō�_���v�Z����ۂɎg�p����j
/// </summary>
/// <param name="start">�J�n�ʒu</param>
/// <param name="end">�I���ʒu</param>
/// <param name="t">�i�s�x</param>
/// <returns>4D�x�N�g��</returns>
inline Vector4D VectorLerp4D(const Vector4D& start, const Vector4D& end, float t)
{
	return {
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t,
		start.w + (end.w - start.w) * t
	};
}