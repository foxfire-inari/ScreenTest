#pragma once
#include "Common.h"
#include "CameraMath.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

struct MeshObject;
class Camera
{
public:
	Camera();
	~Camera();
	void Draw(std::list<MeshObject> worldObjects);
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




	// ���݂̃J�����̑O���x�N�g�� (�����l�̓��[���hZ+)
	Vector3D currentForward;
	// ���݂̃J�����̉E���x�N�g�� (�����l�̓��[���hX+)
	Vector3D currentRight;
	// ���݂̃J�����̏���x�N�g�� (�����l�̓��[���hY+)
	Vector3D currentUp;

};

