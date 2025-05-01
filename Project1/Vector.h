#pragma once
#include <math.h>

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
	/// ���M�𐳋K��
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

};

//�C�����C���֐��F�Z���֐���ʏ��葁�����s�ł���B

/// <summary>
/// �x�N�g�����쐬����
/// </summary>
/// <param name="_x">X���W</param>
/// <param name="_y">Y���W</param>
/// <param name="_z">Z���W</param>
/// <returns>�쐬���ꂽ�x�N�g��</returns>
_inline Vector3D SetVec3D(float _x,float _y,float _z)
{
	Vector3D result;
	result.x = _x;
	result.y = _y;
	result.z = _z;
	return result;
}
