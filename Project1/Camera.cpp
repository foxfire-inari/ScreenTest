#include "Camera.h"
#include "DxLib.h"

namespace
{
	//視点からスクリーンまでの距離
	static const float SCREEN = 1000.f;

	//カメラの移動速度
	static const float MOVE_SPEED = 2.5f;

	//マウス感度
	static const float ROTATION_SENSITIVITY = 5.f;                           
	//補正付きの回転量
	static const float ANGLE_RATE = ONE_DEGREE / ROTATION_SENSITIVITY;    

	//各方向の上限値
	const float MIN_V = -DX_PI_F / 180.0f * 72.5f;
	const float MAX_V = DX_PI_F / 180.0f * 72.5f;
	const float MIN_H = 0.0f;
	const float MAX_H = DX_PI_F * 2.0f;
}


Camera::Camera()
	:location{ 0,0,-SCREEN-100 }
	, rotation{ 0,0,0 }
{
}

Camera::~Camera()
{
}

void Camera::Draw(std::vector< std::vector<Vector3D>> _worldpos)
{
	//描画用の座標を持つ配列
	std::vector<Vector3D> scpos;
	bool isInScreen[2] = { false,false };

	//スクリーンのワールド座標
	Vector3D camsc = location;
	camsc.z += SCREEN;
	DrawFormatString(0, 0, GetColor(255, 255, 255),
		"CameraLocation:(%f,%f,%f)", camsc.x, camsc.y, camsc.z);

	for (int i = 0; i < _worldpos.size(); i++)
	{
		isInScreen[0] = _worldpos[i][0].z > camsc.z;
		isInScreen[1] = _worldpos[i][1].z > camsc.z;
		if (isInScreen[0] == false && isInScreen[1] == false)continue;

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
	//マウスの移動量を保存------------------------マウス関連は別でシングルトンクラスを作成する
	int mouseMoveX = 0;
	int mouseMoveY = 0;

	//現在のマウスの座標を取得
	GetMousePoint(&mouseMoveX, &mouseMoveY);

	//マウスが動いた量を計算
	mouseMoveX -= WINDOW_WIDTH / 2;
	mouseMoveY -= WINDOW_HEIGHT / 2;

	//動いた量から角度を計算
	rotation.x += mouseMoveY * ANGLE_RATE;
	rotation.y += mouseMoveX * ANGLE_RATE;

	//カメラが向ける上限・下限をセット
	if (rotation.x > MAX_V)	rotation.x = MAX_V;
	if (rotation.x < MIN_V)	rotation.x = MIN_V;
	if (rotation.y > MAX_H)	rotation.y -= MAX_H;
	if (rotation.y < MIN_H)	rotation.y += MAX_H;

	if (CheckHitKey(KEY_INPUT_W))location.z += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_S))location.z -= MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_D))location.x += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_A))location.x -= MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_E))location.y += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_Q))location.y -= MOVE_SPEED;

	//マウスの座標を固定
	SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

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
