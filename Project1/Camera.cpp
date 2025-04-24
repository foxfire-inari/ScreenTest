#include "Camera.h"
#include "DxLib.h"

namespace
{
	//���_����X�N���[���܂ł̋���
	static const float SCREEN = 500.f;

	//�J�����̈ړ����x
	static const float MOVE_SPEED = 2.5f;
}


Camera::Camera()
	:location{ 0,0,-500 }
{
}

Camera::~Camera()
{
}

void Camera::Draw(std::vector< std::vector<Vector3D>> _worldpos)
{
	std::vector<Vector3D> scpos;

	for (int i = 0; i < _worldpos.size(); i++)
	{
		for (int j = 0; j < _worldpos[i].size(); j++)
		{
			//�X�N���[�����W�ɕϊ����Ēǉ�
			scpos.emplace_back(GetScreenPos(_worldpos[i][j]));
		}

		//������`��
		DrawLine(scpos[0].x, scpos[0].y,scpos[1].x, scpos[1].y,GetColor(255, 255, 255));

		DrawFormatString(0, 32+32 * i, GetColor(255, 255, 255),
			"SLine:(%f,%f) ELine:(%f,%f)",
			scpos[0].x, scpos[0].y, scpos[1].x, scpos[1].y);

		scpos.clear();
	}
}

void Camera::Update()
{
	if (CheckHitKey(KEY_INPUT_W))location.z += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_S))location.z -= MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_D))location.x += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_A))location.x -= MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_E))location.y += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_Q))location.y -= MOVE_SPEED;

	DrawFormatString(0, 0, GetColor(255, 255, 255),
		"CameraLocation:(%f,%f,%f)", location.x, location.y, location.z);

}

Vector3D Camera::GetScreenPos(Vector3D _worldPos)
{
	//�J�����̍��W�����Z����
	Vector3D worldPos = _worldPos + location;

	//�X�N���[���܂ł̋��������[���h���W�Ŋ���
	float scDis = SCREEN / worldPos.z;

	//scDis�ŏ�Z���邱�Ƃŉ����̂��̂قǒ����Ɋ��A�������Ȃ�B�i���ߊ����ł�j
	Vector3D result;
	result.x = worldPos.x * scDis + WINDOW_WIDTH / 2;
	result.y = worldPos.y * scDis + WINDOW_HEIGHT / 2;
	result.z = 0.f;

	return result;
}
