#pragma once
#include "Common.h"
#include "CameraMath.h"
class Camera
{
public:
	Camera();
	~Camera();
	void Draw(std::vector< std::vector<Vector3D>> _worldpos);
	void Update();

	Vector3D GetPosition() { return position; }
	Vector3D GetScreenPos() { return screenPos; }
	Vector3D GetRotation() { return rotation; }

private:
	//�J�����̍��W
	Vector3D position;

	//�X�N���[���̍��W
	Vector3D screenPos;


	//�J�����̉�](����)
	Vector3D rotation;

	/// <summary>
	/// �X�N���[�����W�ɕϊ�
	/// </summary>
	/// <param name="worldPos">3D��Ԃ̍��W</param>
	/// <returns>�X�N���[�����W</returns>
	Vector3D GetScreenPos(Vector3D worldPos);

};

