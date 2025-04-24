#pragma once
#include <math.h>

/// <summary>
/// �O�����x�N�g��
/// </summary>
struct Vector3D
{
	float x, y, z;

	//���Z(+)
	Vector3D operator +(Vector3D _vec)
	{
		Vector3D vec;
		vec.x = this->x + _vec.x;
		vec.y = this->y + _vec.y;
		vec.z = this->z + _vec.z;
		return vec;
	}

	//���Z(+=)
	void operator +=(Vector3D _vec)
	{
		this->x += _vec.x;
		this->y += _vec.y;
		this->z += _vec.z;
	}

	//���Z(-)
	Vector3D operator -(Vector3D _vec)
	{
		Vector3D vec;
		vec.x = this->x - _vec.x;
		vec.y = this->y - _vec.y;
		vec.z = this->z - _vec.z;
		return vec;
	}
	//���Z(-=)
	void operator -=(Vector3D _vec)
	{
		this->x -= _vec.x;
		this->y -= _vec.y;
		this->z -= _vec.z;
	}

	//float�^�ŃX�P�[�����O(*)
	Vector3D operator *(float _scale)
	{
		Vector3D result;
		result.x = this->x * _scale; 
		result.y = this->y * _scale;
		result.z = this->z * _scale;
		return result;
	}
	//float�^�ŃX�P�[�����O(*=)
	void operator *=(float _scale)
	{
		this->x *= _scale;
		this->y *= _scale;
		this->z *= _scale;
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
