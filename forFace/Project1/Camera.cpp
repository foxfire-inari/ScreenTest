#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"
#include "MeshObject.h"
#include <assert.h>

namespace
{

	/// <summary>
	/// 4D�x�N�g�� start ���� end �ցA�p�����[�^ t (0.0�`1.0) ���g���Đ��`��Ԃ���C�����C���֐� 
	///�i�N���b�s���O�Ō�_���v�Z����ۂɎg�p����j
	/// </summary>
	/// <param name="start">�J�n�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="t">�i�s�x</param>
	/// <returns>4D�x�N�g��</returns>
	inline Vector4D VectorLerp4D(const Vector4D& start, const Vector4D& end, float t)
	{
		return {
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t,
			start.w + (end.w - start.w) * t
		};
	}

	//-----Chohen-Sutherland�A���S���Y��

	//�A�E�g�R�[�h�i�r�b�g���Z���s�����߂̕��j
	static const int INSIDE			=  0;	//000000
	static const int LEFT			=  1;	//000001
	static const int RIGHT			=  2;	//000010
	static const int BOTTOM			=  4;	//000100
	static const int TOP			=  8;	//001000
	static const int OUTCODE_NEAR	= 16;	//010000
	static const int OUTCODE_FAR	= 32;	//100000

	/// <summary>
	/// �N���b�v���W�ɂȂ������A�ǂ̗̈�ɂ��邩�����߂�A�E�g�R�[�h
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
	/// Cohen-Sutherland�A���S���Y�����g�p�����N���b�s���O�֐�
	/// </summary>
	/// <param name="startClip">���`�ϊ����̎n�_</param>
	/// <param name="endClip">���`�ϊ����̏I�_</param>
	/// <returns>�`�悳��邩</returns>
	bool ClipLineCohenSutherland(Vector4D& startClip, Vector4D& endClip)
	{
		//�n�_�I�_�̃A�E�g�R�[�h���Z�b�g
		int startOutcode	= ComputeOutCode(startClip);
		int endOutcode		= ComputeOutCode(endClip);

		//float�̌덷�ɂ�薳�����[�v���������Ă��܂��̂�h������
		const int MAX_ITERATIONS = 10;
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
					//-w<=x��g�ݑւ���x+w<=0�ɂ��čl����
					denominator = direction.x + direction.w;
					//�l�������� =�@" x = -w "�ɂقڕ��s�ɂȂ�A�O���Z�̊댯
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					//��_�͈̔͂��v�Z
					t = (-startClip.x - startClip.w) / denominator;
				}
				else if (outsideCode & RIGHT)
				{
					denominator = direction.x - direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					t = (startClip.w - startClip.x) / denominator;
				}
				else if (outsideCode & BOTTOM)
				{					
					denominator = direction.y + direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }//�����֐���������
					t = (-startClip.y - startClip.w) / denominator;
				}
				else if (outsideCode & TOP)
				{
					denominator = direction.y - direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					t = (startClip.w - startClip.y) / denominator;
				}
				else if (outsideCode & OUTCODE_NEAR)
				{
					//��������0z<=0�Ȃ̂ŏ������킸���ɈႤ
					if (std::fabsf(direction.z) < 1e-6f) { return false; }
					t = -startClip.z / direction.z;
				}
				else if (outsideCode & OUTCODE_FAR)
				{
					denominator = direction.z - direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }
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

	//-----Chohen-Sutherland�A���S���Y���I��

	//�J�����̈ړ����x
	static const float MOVE_SPEED = 2.5f;

	//�}�E�X���x
	static const float ROTATION_SENSITIVITY = ONE_DEGREE * 0.1f;

	//���������̎���p
	static const float VERTICOL_VIEW = 60.0f * ONE_DEGREE;
	//�A�X�y�N�g��
	static const float ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

	//�j�A�N���b�v�ʂ܂ł̋���
	static const float NEAR_Z = 0.1f;
	//�t�@�[�N���b�v�ʂ܂ł̋���
	static const float FAR_Z = 1000.0f;
}


Camera::Camera()
	:position{ 0,0,-50.0f }
	,orientation()
	,currentForward	{ 0.0f, 0.0f, 1.0f }
	,currentRight	{ 1.0f, 0.0f, 0.0f }
	,currentUp		{ 0.0f, 1.0f, 0.0f }
{
	//�J�[�\���𒆉��ɃZ�b�g
	SetMousePoint(static_cast<int>(WINDOW_WIDTH / 2), static_cast<int>(WINDOW_HEIGHT) / 2);

}

Camera::~Camera()
{
}

void Camera::Draw(std::list<MeshObject> worldObjects)
{
	//�K�v�ȍs������O�Ɍv�Z
	Matrix viewMatrix = GetViewMatrix();
	Matrix projMatrix = GetProjectionMatrix();
	Matrix viewProjMatrix = MartixMultiply(viewMatrix, projMatrix);

	//�I�u�W�F�N�g���ƂɎ擾
	for (const auto& object : worldObjects)
	{
		//�I�u�W�F�N�g�̖ʂ���ʂ��擾
		for (const auto& face : object.faceVertexs)
		{
			//�����̎n�_�ƏI�_���N���b�v���W�ɕϊ�
			Vector4D startClip = VEC4Transform({ line.vertexPair.first.x, line.vertexPair.first.y, line.vertexPair.first.z, 1.0f }, viewProjMatrix);
			Vector4D endClip = VEC4Transform({ line.vertexPair.second.x, line.vertexPair.second.y, line.vertexPair.second.z, 1.0f }, viewProjMatrix);

			//�N���b�s���O�����̂��߁A�R�s�[
			Vector4D startClipped = startClip;
			Vector4D endClipped = endClip;

			// Cohen-Sutherland�A���S���Y���Ő������N���b�s���O
			if (ClipLineCohenSutherland(startClipped, endClipped))
			{
				//��������������Ɏc�������̂ݓ���

				//NDC���W�ƃX�N���[�����W���v�Z

				//NDC���W�p
				Vector3D startNDC, endNDC;

				//�p�[�X�y�N�e�B�u���Z�̑O�ɂO���Z�h�~
				if (fabsf(startClipped.w) > 1e-6f && fabsf(endClipped.w) > 1e-6f)
				{
					startNDC = { startClipped.x / startClipped.w,  startClipped.y / startClipped.w ,  startClipped.z / startClipped.w };
					endNDC = { endClipped.x / endClipped.w,    endClipped.y / endClipped.w ,  endClipped.z / endClipped.w };

					//NDC���W���X�N���[�����W�iint�j�ɕϊ�
					float heafWidth = WINDOW_WIDTH / 2.0f;
					float heafHeight = WINDOW_HEIGHT / 2.0f;

					int startX = static_cast<int>(startNDC.x * heafWidth + heafWidth);
					int startY = static_cast<int>(-startNDC.y * heafHeight + heafHeight);//�`���Y���t�ɂ���
					int endX = static_cast<int>(endNDC.x * heafWidth + heafWidth);
					int endY = static_cast<int>(-endNDC.y * heafHeight + heafHeight);//�`���Y���t�ɂ���

					//���̕`���Dxlib�̊֐�
					DrawLine(startX, startY, endX, endY, GetColor(255, 255, 255));

				}
			}
		}
	}
}

void Camera::Update()
{
	//�}�E�X�̌��ݍ��W��ۑ�
	int currentMouseX = 0;
	int currentMouseY = 0;
	GetMousePoint(&currentMouseX, &currentMouseY);

	//��ʒ����̍��W
	const int centerX = static_cast<int>(WINDOW_WIDTH / 2);
	const int centerY = static_cast<int>(WINDOW_HEIGHT / 2);

	//�}�E�X�̈ړ��ʂ�ۑ�
	int mouseMoveX = currentMouseX - centerX;
	int mouseMoveY = currentMouseY - centerY;

	//�}�E�X�̈ړ��ʂ����]�p�x��ۑ��i���W�A���j
	float YawAngle = static_cast<float>(mouseMoveX) * ROTATION_SENSITIVITY;
	float PitchAngle = static_cast<float>(mouseMoveY) * ROTATION_SENSITIVITY;

	float RollAngle = 0.0f;

	if (CheckHitKey(KEY_INPUT_E)) { RollAngle += 2.0f * ONE_DEGREE; }
	if (CheckHitKey(KEY_INPUT_Q)) { RollAngle -= 2.0f * ONE_DEGREE; }

	//�J�����̃��[�J�����W���x�N�g�����擾����
	currentUp = GetUpVector();
	currentRight = GetRightVector();
	currentForward = GetForwardVector();

	//�e��]�����炱�̃t���[���ł̉�]�ʂ�\���N�H�[�^�j�I��
	Quaternion yawDelta   = Quaternion::FromAxisAngle(currentUp,      YawAngle);
	Quaternion pitchDelta = Quaternion::FromAxisAngle(currentRight,   PitchAngle);
	Quaternion rollDelta  = Quaternion::FromAxisAngle(currentForward, RollAngle);

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%f\n%f\n%f\n%f\n", pitchDelta.x,pitchDelta.y,pitchDelta.z,pitchDelta.w);
	//3��Quaternion���������Ă��̃t���[���ł̉�]��\��Quaternion
	Quaternion deltaRotation = rollDelta * pitchDelta * yawDelta;

	//�J�����̌����Ɍv�Z����Quaternion��K��
	orientation = deltaRotation * orientation;

	//�O�̂��ߐ��K��
	orientation.Normalize();

	//-------------�p�x�����������Ȃ炱��

	//-------------��������ړ�����

	//��]��K�p������̎��x�N�g�����擾
	currentForward = GetForwardVector();
	currentRight = GetRightVector();
	currentUp = GetUpVector();

	//�e�����ւ̓��͂�ۑ�
	Vector3D inputVec;
	// �e�����̃C���v�b�g���擾
	if (CheckHitKey(KEY_INPUT_W)) { inputVec.z += 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { inputVec.z -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { inputVec.x += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { inputVec.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_SPACE)) { inputVec.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_LCONTROL)) { inputVec.y -= 1.0f; }

	//��2���@�����
	inputVec.Normalize();

	//���[���h��Ԃ̃x�N�g�����쐬
	Vector3D worldMoveOffset;

	//�e���[�J�����̕����ɓ��͂ƈړ����x�ňړ��x�N�g�����v�Z
	worldMoveOffset += currentForward * inputVec.z * MOVE_SPEED;
	worldMoveOffset += currentRight * inputVec.x * MOVE_SPEED;
	worldMoveOffset += currentUp * inputVec.y * MOVE_SPEED;

	position += worldMoveOffset;

	SetMousePoint(centerX, centerY);

}

Matrix Camera::GetViewMatrix() const
{
	//�J�����̉�]�̋������擾�i�I�u�W�F�N�g�̓J�����Ƌt�ɉ��j
	Quaternion invOrientation = orientation.Conjugate();
	//�N�H�[�^�j�I������]�s��ɕϊ�
	Matrix invRotationMatrix = invOrientation.ToRotationMatrix();
	//�J���������S�ɂȂ�悤�ɕ��s�ړ��s����v�Z
	Matrix invTranslate = Matrix::Identity();
	invTranslate.m[3][0] = -position.x;
	invTranslate.m[3][1] = -position.y;
	invTranslate.m[3][2] = -position.z;

	//���s�ړ��s��*�t��]�s��i���̏��Ԃ�����j
	Matrix viewMatrix = MartixMultiply(invTranslate, invRotationMatrix);

	//�v�Z�����s���Ԃ�
	return viewMatrix;
}

Matrix Camera::GetProjectionMatrix() const
{
	return Perspective(VERTICOL_VIEW,ASPECT,NEAR_Z,FAR_Z);
}

Vector3D Camera::GetForwardVector() const
{
	// ���݂̃J�����̌������s��ɕϊ�
	Matrix rotMat = orientation.ToRotationMatrix();
	//���[���h���W�ł̑O���x�N�g������]�s��ŕϊ�
	return VEC3Transform({ 0.0f, 0.0f, 1.0f }, rotMat);
}

Vector3D Camera::GetRightVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ���[�J�����W�n�̉E���x�N�g������]�s��ŕϊ�
	return VEC3Transform({ 1.0f, 0.0f, 0.0f }, rotMat);
}

Vector3D Camera::GetUpVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ���[�J�����W�n�̏���x�N�g������]�s��ŕϊ�
	return VEC3Transform({ 0.0f, 1.0f, 0.0f }, rotMat);
}
