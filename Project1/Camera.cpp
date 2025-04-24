#include "Camera.h"
#include "DxLib.h"

namespace
{
	//視点からスクリーンまでの距離
	static const float SCREEN = 500.f;

	//カメラの移動速度
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
			//スクリーン座標に変換して追加
			scpos.emplace_back(GetScreenPos(_worldpos[i][j]));
		}

		//線分を描画
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
	//カメラの座標を加算して
	Vector3D worldPos = _worldPos + location;

	//スクリーンまでの距離をワールド座標で割る
	float scDis = SCREEN / worldPos.z;

	//scDisで乗算することで遠くのものほど中央に寄り、小さくなる。（遠近感がでる）
	Vector3D result;
	result.x = worldPos.x * scDis + WINDOW_WIDTH / 2;
	result.y = worldPos.y * scDis + WINDOW_HEIGHT / 2;
	result.z = 0.f;

	return result;
}
