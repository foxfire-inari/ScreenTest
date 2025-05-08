#pragma once
#include <math.h>
#include<DxLib.h>
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

	/// <summary>
	/// �f�o�b�O�p�ɍs��̗v�f��\��
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void DrawData(int x, int y)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				DrawFormatString(x+j*192, y+i*24, GetColor(255, 255, 255), "m.[%d][%d]:%f ", i, j, m[i][j]);
			}
		}
	}

};

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
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += second.m[i][k] * first.m[k][j];
			}
		}
	}
	return result;
}

/// <summary>
/// �������e�s����쐬
/// �i������W�n�j
/// </summary>
/// <param name="viewVer">���������̎���p�i���W�A���j</param>
/// <param name="aspect">�A�X�y�N�g��i��/�����j</param>
/// <param name="_near">�őO�ʂ܂ł̋���</param>
/// <param name="_far">�ŉ��ʂ܂ł̋���</param>
/// <returns>�������e�s��</returns>
static Matrix Perspective(float viewVer, float aspect, float _near, float _far)
{
	//�A�X��A�őO�ʂ܂ł̋������K�؂Ȑ��l���A�ŉ��ʂ��őO�ʂ���O�ɗ��Ă��Ȃ���
	assert(aspect >= 0.0f || _near >= 0.0f || _far >= _near);

	//�V�����s������
	Matrix result;
	//Y�����̊g��
	float yScale = 1.0f / tanf(viewVer / 2.0f);
	//X�����̊g��
	float xScale = yScale / aspect;
	//Z�����̌W��
	float zRange = (_far + _near) / (_far - _near);
	//Z���W�̕ϊ��W��
	float zOffset = -2 * _far * _near * (zRange / (_far + _near));

	//�s��̗v�f���Z�b�g
	result.m[0][0] = xScale;	
	result.m[1][1] = yScale;	
	result.m[2][2] = zRange;	//Z�l��[0,1]�͈̔͂ɋ߂Â���
	result.m[2][3] = 1.0f;		//W��Z�l���R�s�[���邽��
	result.m[3][2] = zOffset;	//���s���Ɋւ��v�Z

	return result;
}
//���ۂ͎����ȗ��ł���炵�����A��w�̂��߂ɏȗ��������\���B