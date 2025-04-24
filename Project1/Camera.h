#pragma once
#include "Common.h"
#include "CameraMath.h"
class Camera
{
public:
	Camera();
	~Camera();
	void Draw(std::vector< std::vector<Vector3D>> _worldvec);
	void Update();

private:
	//�J�����̍��W
	Vector3D location;

	//--------------------------------------�J�����̉�]������


	/// <summary>
	/// �X�N���[�����W�ɕϊ�
	/// </summary>
	/// <param name="worldPos">3D��Ԃ̍��W</param>
	/// <returns>�X�N���[�����W</returns>
	Vector3D GetScreenPos(Vector3D worldPos);

};

