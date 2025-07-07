#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"
#include "MeshObject.h"
#include "ChohenSutherland.h"
#include <assert.h>

namespace
{
	//�J�����̈ړ����x
	static const float MOVE_SPEED = 2.5f;

	//�}�E�X���x
	static const float ROTATION_SENSITIVITY = ONE_DEGREE * 0.1f;
	//���[���̑��x
	static const float ROLL_SPEED = 2.0f * ONE_DEGREE;

	//���������̎���p
	static const float VERTICOL_VIEW = 60.0f * ONE_DEGREE;
	//�A�X�y�N�g��
	static const float ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

	//�j�A�N���b�v�ʂ܂ł̋���
	static const float NEAR_Z = 0.1f;
	//�t�@�[�N���b�v�ʂ܂ł̋���
	static const float FAR_Z = 1000.0f;

	//�������̉�ʒ���
	static const int WINDOW_MID_WIDTH = static_cast<int>(WINDOW_WIDTH / 2);
	//�c�����̉�ʒ���
	static const int WINDOW_MID_HEIGHT = static_cast<int>(WINDOW_HEIGHT / 2);

	//�e�����̊�ɂȂ�x�N�g��
	static const Vector3D FORWARD_VECTOR = { 0.0f, 0.0f, 1.0f };
	static const Vector3D RIGHT_VECTOR	 = { 1.0f, 0.0f, 0.0f };
	static const Vector3D UP_VECTOR		 = { 0.0f, 1.0f, 0.0f };
}


Camera::Camera()
	:position{ 0,0,-50.0f }
	,orientation()
	,currentForward	{ FORWARD_VECTOR }
	,currentRight	{ RIGHT_VECTOR }
	,currentUp		{ UP_VECTOR }
{
	//�J�[�\���𒆉��ɃZ�b�g
	SetMousePoint(WINDOW_MID_WIDTH, WINDOW_MID_HEIGHT);

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
		//�I�u�W�F�N�g�̐�������{���擾
		for (const auto& line : object.lineVertexs)
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
				if (fabsf(startClipped.w) > NEAR_ZERO && fabsf(endClipped.w) > NEAR_ZERO)
				{
					startNDC = { startClipped.x / startClipped.w,  startClipped.y / startClipped.w ,  startClipped.z / startClipped.w };
					endNDC = { endClipped.x / endClipped.w,    endClipped.y / endClipped.w ,  endClipped.z / endClipped.w };

					//��ʂɕ`��ł���悤�Ɍv�Z
					int startX = static_cast<int>(startNDC.x  * WINDOW_MID_WIDTH  + WINDOW_MID_WIDTH);
					int startY = static_cast<int>(-startNDC.y * WINDOW_MID_HEIGHT + WINDOW_MID_HEIGHT);//�`���Y���t�ɂ���
					int endX = static_cast<int>(endNDC.x * WINDOW_MID_WIDTH + WINDOW_MID_WIDTH);
					int endY = static_cast<int>(-endNDC.y * WINDOW_MID_HEIGHT + WINDOW_MID_HEIGHT);//�`���Y���t�ɂ���

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

	//�}�E�X�̈ړ��ʂ�ۑ�
	int mouseMoveX = currentMouseX - WINDOW_MID_WIDTH;
	int mouseMoveY = currentMouseY - WINDOW_MID_HEIGHT;

	//�}�E�X�̈ړ��ʂ����]�p�x��ۑ��i���W�A���j
	float YawAngle = static_cast<float>(mouseMoveX) * ROTATION_SENSITIVITY;
	float PitchAngle = static_cast<float>(mouseMoveY) * ROTATION_SENSITIVITY;

	float RollAngle = 0.0f;

	if (CheckHitKey(KEY_INPUT_E)) { RollAngle += ROLL_SPEED; }
	if (CheckHitKey(KEY_INPUT_Q)) { RollAngle -= ROLL_SPEED; }

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

	SetMousePoint(WINDOW_MID_WIDTH, WINDOW_MID_HEIGHT);
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
	return VEC3Transform(FORWARD_VECTOR, rotMat);
}

Vector3D Camera::GetRightVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ���[�J�����W�n�̉E���x�N�g������]�s��ŕϊ�
	return VEC3Transform(RIGHT_VECTOR, rotMat);
}

Vector3D Camera::GetUpVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ���[�J�����W�n�̏���x�N�g������]�s��ŕϊ�
	return VEC3Transform(UP_VECTOR, rotMat);
}
