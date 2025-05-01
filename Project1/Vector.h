#pragma once
#include <math.h>
#include <assert.h>

//�C�����C���֐��F�Z���֐���ʏ��葁�����s�ł���B

/// <summary>
/// �O�����x�N�g��
/// </summary>
struct Vector3D
{
	float x, y, z;

	//�I�u�W�F�N�g�������ɒl��������
	Vector3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) 
		:x{ _x },y{ _y },z{ _z }{}

	//���Z(+)
	Vector3D operator +(const Vector3D _vec)const { return { x + _vec.x, y + _vec.y, z + _vec.z }; }
	//���Z(+=)
	void operator +=(Vector3D _vec) { x += _vec.x; y += _vec.y; z += _vec.z; }

	//���Z(-)
	Vector3D operator -(const Vector3D _vec)const { return { x - _vec.x, y - _vec.y, z - _vec.z }; }
	//���Z(-=)
	void operator -=(Vector3D _vec) { x -= _vec.x; y -= _vec.y; z -= _vec.z; }

	//float�^�ŃX�P�[�����O(*)
	Vector3D operator*(float _scale) const { return { x * _scale, y * _scale, z * _scale }; }
	//float�^�ŃX�P�[�����O(*=)
	void operator*=(float _scale) { x *= _scale; y *= _scale; z *= _scale; }


	/// <summary>
	/// ��r�p�̒������擾
	///�i�������̌v�Z�͕��ׂ��傫�����߁A�����̔�r�͂�����g���j
	/// </summary>
	/// <returns>��r�p�̒���</returns>
	float Length() { return x*x + y*y + z*z; }

	/// <summary>
	/// �x�N�g���̒������擾
	/// </summary>
	/// <returns>�x�N�g���̒���</returns>
	float LengthSq() { return sqrtf(Length()); }

	/// <summary>
	/// ���g�𐳋K��
	/// </summary>
	void Normalize()
	{
		//���g�̒������擾
		float length = LengthSq();

		//float�^�����m�ɕ\���ł��錅�i23�r�b�g��7���j
		//���ɏ������l���傫�������m�F���Ă���v�Z����i�O���Z�����Ȃ����߁j
		if (length > 1e-12f)
		{
			//���Z�Ə�Z�ł͏��Z�̕������ׂ��傫�����߁A�t�����擾���Ă���
			float invLen = 1.0f / length;

			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
		//���ɏ������l�Ȃ�[���x�N�g���ɂ���
		else { x = 0; y = 0; x = 0; }
	}

	/// <summary>
	/// ���g�𐳋K�������Ƃ��̃x�N�g���𐶐����ĕԂ�
	/// </summary>
	/// <returns>���K�����ꂽ�x�N�g��</returns>
	Vector3D Normalized() const
	{
		//�����̃R�s�[���쐬
		Vector3D result = *this;
		//�R�s�[�𐳋K��
		result.Normalize();
		//���K�����ꂽ�R�s�[��Ԃ�
		return result;
	}

};

/// <summary>
/// �x�N�g���̓��ς����߂�
/// </summary>
/// <param name="aVec">�����n�_�����x�N�g��a</param>
/// <param name="bVec">�����n�_�����x�N�g��b</param>
/// <returns>����</returns>
inline float Dot(const Vector3D aVec, const Vector3D bVec)
{
	return aVec.x*bVec.x + aVec.y*bVec.y + aVec.z*bVec.z;
}

/// <summary>
/// �x�N�g���̊O�ς����߂�
/// </summary>
/// <param name="rightVec">�E�ӂɂ�����x�N�g��</param>
/// <param name="leftVec">���ӂɂ�����x�N�g��</param>
/// <returns>�O��</returns>
static Vector3D Cross(const Vector3D rightVec, const Vector3D leftVec)
{
	Vector3D result;
	result.x = rightVec.y*leftVec.z - rightVec.z*leftVec.y;
	result.y = rightVec.z*leftVec.x - rightVec.x*leftVec.z;
	result.z = rightVec.x*leftVec.y - rightVec.y*leftVec.x;
	return result;
}

static float AngleOfVector(const Vector3D aVec, const Vector3D bVec)
{
	Vector3D aLength = aVec.Normalized();
	Vector3D bLength = bVec.Normalized();
	assert

}