#pragma once
#include "Common.h"
#include "CameraMath.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

class Camera
{
public:
	Camera();
	~Camera();
	void Draw(std::vector< std::vector<Vector3D>> _worldpos);
	void Update();

	Vector3D GetPosition() { return position; }

	/// <summary>
	/// �r���[�s����擾����
	///�i���[���h���W����J�������W�ւ̕ϊ��p�s��j
	/// </summary>
	/// <returns>�r���[�s��</returns>
	Matrix GetViewMatrix() const;

	/// <summary>
	/// �v���W�F�N�V�����s����擾����
	///�i�������e���s���s��j
	/// </summary>
	/// <returns>�v���W�F�N�V�����s��</returns>
	Matrix GetProjectionMatrix() const;


	/// <summary>
	/// �J�����̐��ʂ̃x�N�g�������[���h���W�ɕϊ�
	/// </summary>
	/// <returns>�J�����̐��ʁi���[���h���W�j</returns>
	Vector3D GetForwardVector()const;
	
	/// <summary>
	/// �J�����̉E�̃x�N�g�������[���h���W�ɕϊ�
	/// </summary>
	/// <returns>�J�����̉E�i���[���h���W�j</returns>
	Vector3D GetRightVector()const;
	
	/// <summary>
	/// �J�����̏�̃x�N�g�������[���h���W�ɕϊ�
	/// </summary>
	/// <returns>�J�����̏�i���[���h���W�j</returns>
	Vector3D GetUpVector()const;

private:
	//�J�����̃��[���h���W
	Vector3D position;
	//�J�����̉�](����)
	Quaternion orientation;

	//���������̎���p
	float viewVer = 60.0f * ONE_DEGREE;
	//�A�X�y�N�g��
	float aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
	//�j�A�N���b�v�ʂ܂ł̋���
	float nearZ = 0.1f;
	//�t�@�[�N���b�v�ʂ܂ł̋���
	float farZ = 1000.0f;

	// ���݂̃J�����̑O���x�N�g�� (�����l�̓��[���hZ+)
	Vector3D currentForward = { 0.0f, 0.0f, 1.0f };
	// ���݂̃J�����̉E���x�N�g�� (�����l�̓��[���hX+)
	Vector3D currentRight = { 1.0f, 0.0f, 0.0f };
	// ���݂̃J�����̏���x�N�g�� (�����l�̓��[���hY+)
	Vector3D currentUp = { 0.0f, 1.0f, 0.0f };

};

