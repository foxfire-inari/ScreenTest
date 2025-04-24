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
};

// X����]�s��
static Matrix GetMatrixAxisX(float XAxisRotation)
{
	float sin, cos;
	sin = sinf(XAxisRotation);
	cos = cosf(XAxisRotation);

	Matrix result =
	{
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f},
			{ 0.0f,  cos,  sin, 0.0f},
			{ 0.0f, -sin,  cos, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	return result;
}

// Y����]�s��
static Matrix GetMatrixAxisY(float YAxisRotation)
{
	float sin, cos;
	sin = sinf(YAxisRotation);
	cos = cosf(YAxisRotation);

	Matrix result =
	{
		{
			{  cos, 0.0f, -sin, 0.0f},
			{ 0.0f, 1.0f, 0.0f, 0.0f},
			{  sin, 0.0f,  cos, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	return result;
}