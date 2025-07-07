#pragma once
#include "DxLib.h"
#include "CameraMath.h"
#include "MeshObject.h"
#include <assert.h>

namespace
{
	//�A�E�g�R�[�h�i�r�b�g���Z���s�����߂̕��j
	static const int INSIDE = 0;	//000000
	static const int LEFT = 1;	//000001
	static const int RIGHT = 2;	//000010
	static const int BOTTOM = 4;	//000100
	static const int TOP = 8;	//001000
	static const int OUTCODE_NEAR = 16;	//010000
	static const int OUTCODE_FAR = 32;	//100000

	//float�̌덷�ɂ�薳�����[�v���������Ă��܂��̂�h������
	static const int MAX_ITERATIONS = 10;

	/// <summary>
	/// �N���b�v���W�ɂȂ������A�ǂ̗̈�ɂ��邩��߂�A�E�g�R�[�h
	/// </summary>
	/// <param name="p">�_P</param>
	/// <returns>�_P�����݂�����</returns>
	int ComputeOutCode(const Vector4D& p)
	{
		//�����Ƃ��ď�����
		int code = INSIDE;

		//W���O��菬�������͂�����O�ɂ��邱�Ƃ������m�肵�Ă���
		//X,Y�̓N���b�s���O������ɂ�����x��
		if (p.w < 0.0f)
		{
			code |= OUTCODE_NEAR;
			return code;
		}

		// �㉺���E�\���Ŕ�r���A�O���ɂ�������Ή�����r�b�g�𗧂Ă�

		//�����ɋN���肦�Ȃ��̂�else�ł܂Ƃ߂�
		if (p.x < -p.w) { code |= LEFT; }
		else if (p.x > p.w) { code |= RIGHT; }

		if (p.y < -p.w) { code |= BOTTOM; }
		else if (p.y > p.w) { code |= TOP; }

		if (p.z < 0.0f) { code |= OUTCODE_NEAR; }
		else if (p.z > p.w) { code |= OUTCODE_FAR; }

		return code;
	}

	/// <summary>
	/// �O���Z�̊댯���Ȃ����`�F�b�N����
	/// </summary>
	/// <param name="_denominator">�`�F�b�N����l</param>
	/// <returns>�댯�����邩</returns>
	bool DenominatorChack(float _denominator)
	{
		return std::fabsf(_denominator) < NEAR_ZERO;
	}

	/// <summary>
	/// Cohen-Sutherland�A���S���Y�����g�p�����N���b�s���O�֐�
	/// </summary>
	/// <param name="startClip">���`�ϊ����̎n�_</param>
	/// <param name="endClip">���`�ϊ����̏I�_</param>
	/// <returns>�`�悳��邩</returns>
	bool ClipLineCohenSutherland(Vector4D& startClip, Vector4D& endClip)
	{
		//�n�_�I�_�̃A�E�g�R�[�h���Z�b�g
		int startOutcode = ComputeOutCode(startClip);
		int endOutcode = ComputeOutCode(endClip);


		//���݂̔�����
		int iterations = 0;

		while (iterations < MAX_ITERATIONS)
		{
			iterations++;

			//�r�b�gor������0�Ȃ痼�[�������ɂ���
			if ((startOutcode | endOutcode) == 0)
			{
				//�����S�̂�\��
				return true;
			}
			//���[�_�������O���̈�i�E���≺�����j�Ȃ�\������Ȃ�
			else if ((startOutcode & endOutcode) != 0)
			{
				return false;
			}
			//�N���b�s���O���K�v�ȏꍇ
			else
			{
				//�O���ɂ�����̓_�̃A�E�g�R�[�h��I��
				int outsideCode = (startOutcode != 0) ? startOutcode : endOutcode;

				//��`�Ƃ̌�_������startClipendClip�̂ǂ͈̔͂ɂ��邩
				//�i0.0f<t<1.0f�j�̏ꍇ�͐���startClipendClip�̂ǂ���
				float t = 0.0f;

				//��_�̍��W
				Vector4D intersection;

				//�����̕����x�N�g��
				Vector4D direction = {
					endClip.x - startClip.x,
					endClip.y - startClip.y,
					endClip.z - startClip.z,
					endClip.w - startClip.w
				};

				//����Z�̕���
				float denominator;

				//outsideCode�ɑ΂��鋫�E���ʁi�`�悳���͈͂̋��E�j�Ƃ̌���t���v�Z
				if (outsideCode & LEFT)
				{
					// -w<=x��g�ݑւ���x+w<=0�ɂ��čl����
					denominator = direction.x + direction.w;
					//�l�������� =�@" x = -w "�ɂقڕ��s�ɂȂ�A�O���Z�̊댯
					if (DenominatorChack(denominator)) { return false; }
					//��_�͈̔͂��v�Z
					t = (-startClip.x - startClip.w) / denominator;
				}
				else if (outsideCode & RIGHT)
				{
					// x<=w ���� x-w<=0
					denominator = direction.x - direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (startClip.w - startClip.x) / denominator;
				}
				else if (outsideCode & BOTTOM)
				{
					// -w<=y ���� y+w<=0
					denominator = direction.y + direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (-startClip.y - startClip.w) / denominator;
				}
				else if (outsideCode & TOP)
				{
					// y<=w ���� y-w<=0
					denominator = direction.y - direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (startClip.w - startClip.y) / denominator;
				}
				else if (outsideCode & OUTCODE_NEAR)
				{
					//�������� z<=0 �Ȃ̂ŏ������킸���ɈႤ
					if (DenominatorChack(direction.z)) { return false; }
					t = -startClip.z / direction.z;
				}
				else if (outsideCode & OUTCODE_FAR)
				{
					// z<=w ���� z-w<=0
					denominator = direction.z - direction.w;
					if (DenominatorChack(denominator)) { return false; }
					t = (startClip.w - startClip.z) / denominator;
				}
				//�����܂ŗ����牽�炩�̃o�O���N���Ă��邱�ƂɂȂ�
				else { assert(false); }

				//t��������͈̔͂ɂȂ��ꍇ�͌v�Z�����Ȃ�
				if (t < 0.0f || t>1.0f) { return false; }

				//��_�̍��W���v�Z
				intersection = VectorLerp4D(startClip, endClip, t);

				//�O���̓_����_�ɒu�������A���̓_��outcode���v�Z���Ȃ���
				if (outsideCode == startOutcode)
				{
					startClip = intersection;
					startOutcode = ComputeOutCode(startClip);
				}
				else
				{
					endClip = intersection;
					endOutcode = ComputeOutCode(endClip);
				}
			}
		}
		//�ő�܂ŉ��̂�float������������
		return false;

	}
}