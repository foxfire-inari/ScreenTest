#pragma once
#include <cmath>
#include "Vector.h"
#include "Matrix.h"

/*
* �I�C���[�p�͏��Ԃɉ�]������s����W���o�����b�N�Ƃ�����ԂɂȂ�
* Y��X��Z�̏��Ԃŉ�]����ꍇ�A����X��90�x��]�����Ƃ�����
* Y��Z�̉�]�����������ɂȂ��Ă��܂��B
* https://www.youtube.com/watch?v=zc8b2Jo7mno
* ���̓��悪��ԃW���o�����b�N�����o�I�ɗ������₷�������B
*
* �N�I�[�^�j�I���́ix,y,z,w�j�Ƃ����v�f�łł��Ă���
* XYZ�͉�]�̎��AW�͉�]�̗ʂ�\���Ă���
* x^2+y^2+z^2+w^2=1
*/

/// <summary>
/// �N�I�[�^�j�I��
/// </summary>
struct Quaternion
{
	float x, y, z, w;
	//�������ɒP�ʃN�I�[�^�j�I���ŏ������ł���R���X�g���N�^
	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	//�������Ɋe�������Z�b�g���ď���������R���X�g���N�^
	Quaternion(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w) {}

	/// <summary>
	/// �P�ʃN�I�[�^�j�I����Ԃ�
	/// </summary>
	/// <returns>�P�ʃN�I�[�^�j�I��</returns>
	static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

    /// <summary>
    /// axis��angle������]����N�I�[�^�j�I���𐶐�
    /// </summary>
    /// <param name="axis">��</param>
    /// <param name="angle">��]�ʁi���W�A���j</param>
    /// <returns>�N�I�[�^�j�I��</returns>
    static Quaternion FromAxisAngle(const Vector3D& axis, float angle) 
	{
		//���̓s�����]�ʂ𔼌�������K�v������
        float halfAngle = angle * 0.5f;
        float sin = sinf(halfAngle);
        float cos = cosf(halfAngle);

		// ��]���͕K�����K������
        Vector3D normalizedAxis = axis.Normalized(); 

		//�v�Z���ʂ��N�I�[�^�j�I���Ƃ��ĕۑ�
        Quaternion result(normalizedAxis.x * sin, normalizedAxis.y * sin, normalizedAxis.z * sin, cos);

        return result;
    }

	//�����͂�������

};







