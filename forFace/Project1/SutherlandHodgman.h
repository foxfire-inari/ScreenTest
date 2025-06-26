#pragma once
#include "DxLib.h"
#include "CameraMath.h"
#include "FaceObject.h"
#include "Quaternion.h"
#include <assert.h>

namespace
{
	/// <summary>
	/// �͂ݏo�Ă������
	/// </summary>
	enum OutVec
	{
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,
		OUTVEC_FAR,
		OUTVEC_NEAR
	};

	/// <summary>
	/// �O���Z�̊댯���Ȃ����`�F�b�N����
	/// </summary>
	/// <param name="_denominator">�`�F�b�N����l</param>
	/// <returns>�댯�����邩</returns>
	bool DenominatorChack(float _denominator)
	{
		return std::fabsf(_denominator) < NEAR_ZERO;
	}

	Vector4D intersect(const Vector4D& startClip, const Vector4D& endClip, OutVec outvec)
	{
		//�N���b�v��̍��W
		Vector4D result;

		//��`�Ƃ̌�_������startClipendClip�̂ǂ͈̔͂ɂ��邩
		//�i0.0f<t<1.0f�j�̏ꍇ�͐���startClipendClip�̂ǂ���
		float t = 0.0f;

		//�����̕����x�N�g��
		Vector4D direction = {
			endClip.x - startClip.x,
			endClip.y - startClip.y,
			endClip.z - startClip.z,
			endClip.w - startClip.w
		};
		//����Z�̕���
		float denominator;

		//����ʂɑ΂��鋫�E���ʁi�`�悳���͈͂̋��E�j�Ƃ̌���t���v�Z
		switch (outvec)
		{
		case LEFT:
			// -w<=x��g�ݑւ���x+w<=0�ɂ��čl����
			denominator = direction.x + direction.w;
			//�l�������� =�@" x = -w "�ɂقڕ��s�ɂȂ�A�O���Z�̊댯
			if (DenominatorChack(denominator)) { return false; }
			//��_�͈̔͂��v�Z
			t = (-startClip.x - startClip.w) / denominator;
			break;
		case RIGHT:
			// x<=w ���� x-w<=0
			denominator = direction.x - direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (startClip.w - startClip.x) / denominator;
			break;
		case BOTTOM:
			// -w<=y ���� y+w<=0
			denominator = direction.y + direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (-startClip.y - startClip.w) / denominator;
			break;
		case TOP:
			// y<=w ���� y-w<=0
			denominator = direction.y - direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (startClip.w - startClip.y) / denominator;
			break;
		case OUTVEC_NEAR:
			//��������0z<=0�Ȃ̂ŏ������킸���ɈႤ
			if (DenominatorChack(direction.z)) { return false; }
			t = -startClip.z / direction.z;
			break;
		case OUTVEC_FAR:
			// z<=w ���� z-w<=0
			denominator = direction.z - direction.w;
			if (DenominatorChack(denominator)) { return false; }
			t = (startClip.w - startClip.z) / denominator;
			break;
		default:
			//�����܂ŗ����牽�炩�̃o�O���N���Ă��邱�ƂɂȂ�
			assert(false);
		}

		//t��������͈̔͂ɂȂ��ꍇ�͌v�Z�����Ȃ�
		if (t < 0.0f || t>1.0f) { return false; }

		//��_�̍��W���v�Z
		result = VectorLerp4D(startClip, endClip, t);

		return result;
	}

	/// <summary>
	/// �����ɂ��邩
	/// </summary>
	/// <param name="p">���_</param>
	/// <param name="outvec">�m�F�����</param>
	/// <returns>�͂ݏo�Ă��邩�ǂ���</returns>
	bool inside(const Vector4D& p, OutVec outvec)
	{
		switch (outvec)
		{
		case LEFT:			return p.x > -p.w;
		case RIGHT:			return p.x < p.w;
		case BOTTOM:		return p.y > -p.w;
		case TOP:			return p.y < p.w;
		case OUTVEC_NEAR:	return p.z > -p.w;
		case OUTVEC_FAR:	return p.z < p.w;
		}
		return false;
	}


	/// <summary>
	/// �t�F�[�X���N���b�v���A�V�����t�F�[�X�����
	/// </summary>
	/// <param name="face">�t�F�[�X</param>
	/// <param name="outvec">�ΏۂƂ���N���b�v��</param>
	/// <returns>�N���b�v��̃t�F�[�X</returns>
	std::vector<Vector4D> clipEdge(const std::vector<Vector4D>& face, OutVec outvec)
	{
		std::vector<Vector4D> result;

		//�t�F�[�X�̕ӂ�����v�Z
		for (int i = 0; i < face.size(); ++i)
		{
			Vector4D startvertex = face[i];
			Vector4D endvertex = face[(i + 1) % face.size()];

			bool startIn = inside(startvertex, outvec);
			bool endIn = inside(endvertex, outvec);

			if (startIn && endIn)
			{
				result.push_back(endvertex);
			}
			else if (startIn && !endIn)
			{
				result.push_back(intersect(startvertex, endvertex, outvec));
			}
			else if (!startIn && endIn)
			{
				result.push_back(intersect(startvertex, endvertex, outvec));
				result.push_back(endvertex);
			}
		}
		return result;
	}

	/// <summary>
	/// �t�F�[�X���N���b�v�͈͂Ɏ��߂鏈��
	/// </summary>
	/// <param name="face">�t�F�[�X</param>
	/// <returns>�N���b�v��̃t�F�[�X</returns>
	std::vector<Vector4D> sutherlandHodgmanClip(const std::vector<Vector4D>& face)
	{
		std::vector<Vector4D> result = face;

		//�e�N���b�v�ʂŐ���������
		for (OutVec e : {LEFT, RIGHT, BOTTOM, TOP, OUTVEC_FAR, OUTVEC_NEAR})
		{
			if (result.size() == 0)return result;
			result = clipEdge(result, e);
		}
		return result;
	}

	/// <summary>
	/// �p�[�X�y�N�e�B�u���Z
	/// </summary>
	/// <param name="face">�N���b�v�ς݂�Vector4D�z��</param>
	/// <returns>NDC���W�̔z��</returns>
	FaceVertex ChangeNDC(std::vector<Vector4D> face)
	{
		FaceVertex result;

		Vector3D vec;

		for (int i = 0; i < face.size(); i++)
		{
			vec = { face[i].x / face[i].w,face[i].y / face[i].w,face[i].z / face[i].w };
			result.vertexs.push_back(vec);
		}
		return result;
	}

	/// <summary>
	/// �t�F�[�X���O�p�`�ɕ�����
	/// </summary>
	/// <param name="face">�N���b�v���ꂽ�t�F�[�X</param>
	/// <returns>�O�p�`�̃t�F�[�X�z��</returns>
	std::vector<FaceVertex> Triangulate(const FaceVertex& face)
	{
		std::vector<FaceVertex> result;

		if (face.vertexs.size() < 3)return result;

		for (int i = 1; i + 1 < face.vertexs.size(); ++i)
		{
			FaceVertex triangle = { { face.vertexs[0],face.vertexs[i],face.vertexs[i + 1] },face.normalVer };
			result.push_back(triangle);
		}
		return result;
	}
}