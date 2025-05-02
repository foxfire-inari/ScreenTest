#pragma once
#include <math.h>

/// <summary>
/// �O�����̐����s��
/// w�܂ł���̂�
/// �x�N�g���̕��s�ړ��ϊ��Ŏg������
/// </summary>
struct Matrix
{
	float m[4][4];

	//�I�u�W�F�N�g�������ɒl��������
	Matrix() 
	{
		for (int i = 0; i < 4; ++i) 
		{
			for (int j = 0; j < 4; ++j) 
			{
				m[i][j] = 0.0f;
			}
		}
	}

	/// <summary>
	/// �P�ʍs���Ԃ�
	/// </summary>
	/// <returns>�P�ʍs��</returns>
	static Matrix Identity()
	{
		Matrix result;
		result.m[0][0] = 1.0f;
		result.m[1][1] = 1.0f;
		result.m[2][2] = 1.0f;
		result.m[3][3] = 1.0f;
		return result;
	}

};

/// <summary>
/// X���̉�]�s��𐶐�����֐�
/// �i������W�n�j
/// </summary>
/// <param name="XAxisRotation">�p�x�i���W�A���j</param>
/// <returns>X����]�s��</returns>
static Matrix GetMatrixAxisX(float XAxisRotation)
{
	float sin = sinf(XAxisRotation);
	float cos = cosf(XAxisRotation);

	//�P�ʍs����쐬
	Matrix result = Matrix::Identity();
	//��]�s��̗v�f��ݒ�
	result.m[1][1] =  cos; result.m[1][2] = sin;
	result.m[2][1] = -sin; result.m[2][2] = cos;

	//{ 1.0f, 0.0f, 0.0f, 0.0f }
	//{ 0.0f,  cos,  sin, 0.0f }
	//{ 0.0f, -sin,  cos, 0.0f }
	//{ 0.0f, 0.0f, 0.0f, 1.0f }

	return result;
}

/// <summary>
/// Y���̉�]�s��𐶐�����֐�
/// �i������W�n�j
/// </summary>
/// <param name="YAxisRotation">�p�x�i���W�A���j</param>
/// <returns>Y����]�s��</returns>
static Matrix GetMatrixAxisY(float YAxisRotation)
{
	float sin = sinf(YAxisRotation);
	float cos = cosf(YAxisRotation);

	//�P�ʍs����쐬
	Matrix result = Matrix::Identity();
	//��]�s��̗v�f��ݒ�
	result.m[0][0] =  cos; result.m[0][2] = sin;
	result.m[2][0] = -sin; result.m[2][2] = cos;

	// {  cos, 0.0f, -sin, 0.0f}
	// { 0.0f, 1.0f, 0.0f, 0.0f}
	// {  sin, 0.0f,  cos, 0.0f}
	// { 0.0f, 0.0f, 0.0f, 1.0f}

	return result;
}

/// <summary>
/// Z���̉�]�s����Đ�����֐�
/// �i������W�n�j
/// </summary>
/// <param name="ZAxisRotation">�p�x�i���W�A���j</param>
/// <returns>Z����]�s��</returns>
static Matrix GetMatrixAxisZ(float ZAxisRotation)
{
	float sin = sinf(ZAxisRotation);
	float cos = cosf(ZAxisRotation);

	//�P�ʍs����쐬
	Matrix result = Matrix::Identity();
	//��]�s��̗v�f��ݒ�
	result.m[0][0] = cos; result.m[0][1] = -sin;
	result.m[1][0] = sin; result.m[1][1] = cos;

	return result;
}

/// <summary>
/// �s��̏�Z
/// </summary>
/// <param name="second">��Ɍv�Z�����s��</param>
/// <param name="first">��Ɍv�Z�����s��</param>
/// <returns>�s��̐�</returns>
static Matrix MartixMultiply(const Matrix& second, const Matrix& first)
{
	Matrix result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += second.m[i][k] * first.m[k][j];
			}
		}
	}
	return result;
}

//------------------��������J�n����