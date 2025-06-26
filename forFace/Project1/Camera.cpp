#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"
#include "FaceObject.h"
#include "SutherlandHodgman.h"
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
	static const Vector3D RIGHT_VECTOR = { 1.0f, 0.0f, 0.0f };
	static const Vector3D UP_VECTOR = { 0.0f, 1.0f, 0.0f };
}


Camera::Camera()
	:position{ 0,0,-50.0f }
	,orientation()
	,currentForward	{ 0.0f, 0.0f, 1.0f }
	,currentRight	{ 1.0f, 0.0f, 0.0f }
	,currentUp		{ 0.0f, 1.0f, 0.0f }
{
	//�J�[�\���𒆉��ɃZ�b�g
	SetMousePoint(WINDOW_MID_WIDTH, WINDOW_MID_HEIGHT);

}

Camera::~Camera()
{
}

void Camera::Draw(std::list<FaceObject> worldObjects)
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
			//�t�F�[�X����`�ϊ�����
			std::vector<Vector4D> TransformedFace = TransformFace(face,viewProjMatrix);

			//�t�F�[�X���N���b�v����
			std::vector<Vector4D> ClipedFace = sutherlandHodgmanClip(TransformedFace);

			//���g���Ȃ��Ȃ玟�ɍs��
			if (ClipedFace.size() <= 0)continue;

			//�p�[�X�y�N�e�B�u���Z
			FaceVertex NDCFace = ChangeNDC(ClipedFace);
			
			//�S�Ẵt�F�[�X���O�p�`�ɂ���
			std::vector<FaceVertex> triangles = Triangulate(NDCFace);

			for (const auto& triangle : triangles)
			{
				//��ʂɕ`��ł���悤�Ɍv�Z
				int startvertexX = static_cast<int>(triangle.vertexs[0].x  * WINDOW_MID_WIDTH  + WINDOW_MID_WIDTH);
				int startvertexY = static_cast<int>(-triangle.vertexs[0].y * WINDOW_MID_HEIGHT + WINDOW_MID_HEIGHT);
				int midvertexX = static_cast<int>(triangle.vertexs[1].x  * WINDOW_MID_WIDTH  + WINDOW_MID_WIDTH);
				int midvertexY = static_cast<int>(-triangle.vertexs[1].y * WINDOW_MID_HEIGHT + WINDOW_MID_HEIGHT);
				int endvertexX = static_cast<int>(triangle.vertexs[2].x  * WINDOW_MID_WIDTH  + WINDOW_MID_WIDTH);
				int endvertexY = static_cast<int>(-triangle.vertexs[2].y * WINDOW_MID_HEIGHT + WINDOW_MID_HEIGHT);

				DrawTriangle(startvertexX, startvertexY, midvertexX, midvertexY, endvertexX, endvertexY, GetColor(255,255,255), true);
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

