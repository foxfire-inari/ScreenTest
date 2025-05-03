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
    /// axis������angle������]����N�I�[�^�j�I���𐶐�
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

	/// <summary>
	/// �N�H�[�^�j�I���̏�Z
	/// </summary>
	/// <param name="qua">�N�H�[�^�j�I��</param>
	/// <returns>this*qua</returns>
	Quaternion operator*(const Quaternion& qua)const
	{
		Quaternion result;
		result.w = w * qua.w - x * qua.x - y * qua.y - z * qua.z;
		result.x = w * qua.x + x * qua.w + y * qua.z - z * qua.y;
		result.y = w * qua.y - x * qua.z + y * qua.w + z * qua.x;
		result.z = w * qua.z + x * qua.y - y * qua.x + z * qua.w;
		return result;
	}

	/// <summary>
	/// �N�H�[�^�j�I���̏�Z
	/// </summary>
	/// <param name="qua">�N�H�[�^�j�I��</param>
	/// <returns>this*qua</returns>
	Quaternion& operator*=(const Quaternion& qua)
	{
		*this = *this * qua;
		return *this;
	}

	/// <summary>
	/// ��r�p�̒������擾�i�����̓������߂�j
	///�i�������̌v�Z�͕��ׂ��傫�����߁A�����̔�r�͂�����g���j
	/// </summary>
	/// <returns>��r�p�̒���</returns>
	float LengthSq()const { return x * x + y * y + z * z + w * w; }

	/// <summary>
	/// �N�H�[�^�j�I���̒������擾
	/// </summary>
	/// <returns>�x�N�g���̒���</returns>
	float Length()const { return sqrtf(LengthSq()); }

	/// <summary>
	/// ���g�𐳋K��
	/// </summary>
	void Normalize()
	{
		//���g�̒������擾
		float length = Length();

		//float�^�����m�ɕ\���ł��錅�i23�r�b�g��7���j
		//���ɏ������l���傫�������m�F���Ă���v�Z����i�O���Z�����Ȃ����߁j
		if (length > 1e-12f)
		{
			//���Z�Ə�Z�ł͏��Z�̕������ׂ��傫�����߁A�t�����擾���Ă���
			float invLen = 1.0f / length;

			x *= invLen;
			y *= invLen;
			z *= invLen;
			w *= invLen;
		}
		//���ɏ������l�Ȃ�P�ʃN�H�[�^�j�I���ɂ���
		else { *this = Identity(); }
	}

	/// <summary>
	/// ���g�𐳋K�������Ƃ��̃N�H�[�^�j�I���𐶐����ĕԂ�
	/// </summary>
	/// <returns>���K�����ꂽ�N�H�[�^�j�I��</returns>
	Quaternion Normalized()const
	{
		//�����̃R�s�[���쐬
		Quaternion result = *this;
		//�R�s�[�𐳋K��
		result.Normalize();
		//���K�����ꂽ�R�s�[��Ԃ�
		return result;
	}

	/// <summary>
	/// �����N�H�[�^�j�I���𐶐����ĕԂ�
	/// </summary>
	/// <returns>�����N�H�[�^�j�I��</returns>
	Quaternion Conjugate() const { return Quaternion(-x, -y, z, w); }

	//���͂�������

};







