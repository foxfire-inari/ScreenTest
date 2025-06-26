#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"
#include "FaceObject.h"
#include "SutherlandHodgman.h"
#include <assert.h>

namespace
{
	//カメラの移動速度
	static const float MOVE_SPEED = 2.5f;

	//マウス感度
	static const float ROTATION_SENSITIVITY = ONE_DEGREE * 0.1f;
	//ロールの速度
	static const float ROLL_SPEED = 2.0f * ONE_DEGREE;

	//垂直方向の視野角
	static const float VERTICOL_VIEW = 60.0f * ONE_DEGREE;
	//アスペクト比
	static const float ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

	//ニアクリップ面までの距離
	static const float NEAR_Z = 0.1f;
	//ファークリップ面までの距離
	static const float FAR_Z = 1000.0f;

	//横方向の画面中央
	static const int WINDOW_MID_WIDTH = static_cast<int>(WINDOW_WIDTH / 2);
	//縦方向の画面中央
	static const int WINDOW_MID_HEIGHT = static_cast<int>(WINDOW_HEIGHT / 2);

	//各方向の基準になるベクトル
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
	//カーソルを中央にセット
	SetMousePoint(WINDOW_MID_WIDTH, WINDOW_MID_HEIGHT);

}

Camera::~Camera()
{
}

void Camera::Draw(std::list<FaceObject> worldObjects)
{
	//必要な行列を事前に計算
	Matrix viewMatrix = GetViewMatrix();
	Matrix projMatrix = GetProjectionMatrix();
	Matrix viewProjMatrix = MartixMultiply(viewMatrix, projMatrix);

	//オブジェクトごとに取得
	for (const auto& object : worldObjects)
	{
		//オブジェクトの面を一面ずつ取得
		for (const auto& face : object.faceVertexs)
		{
			//フェースを線形変換する
			std::vector<Vector4D> TransformedFace = TransformFace(face,viewProjMatrix);

			//フェースをクリップする
			std::vector<Vector4D> ClipedFace = sutherlandHodgmanClip(TransformedFace);

			//中身がないなら次に行く
			if (ClipedFace.size() <= 0)continue;

			//パースペクティブ除算
			FaceVertex NDCFace = ChangeNDC(ClipedFace);
			
			//全てのフェースを三角形にする
			std::vector<FaceVertex> triangles = Triangulate(NDCFace);

			for (const auto& triangle : triangles)
			{
				//画面に描画できるように計算
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
	//マウスの現在座標を保存
	int currentMouseX = 0;
	int currentMouseY = 0;
	GetMousePoint(&currentMouseX, &currentMouseY);


	//マウスの移動量を保存
	int mouseMoveX = currentMouseX - WINDOW_MID_WIDTH;
	int mouseMoveY = currentMouseY - WINDOW_MID_HEIGHT;

	//マウスの移動量から回転角度を保存（ラジアン）
	float YawAngle = static_cast<float>(mouseMoveX) * ROTATION_SENSITIVITY;
	float PitchAngle = static_cast<float>(mouseMoveY) * ROTATION_SENSITIVITY;

	float RollAngle = 0.0f;

	if (CheckHitKey(KEY_INPUT_E)) { RollAngle += ROLL_SPEED; }
	if (CheckHitKey(KEY_INPUT_Q)) { RollAngle -= ROLL_SPEED; }

	//カメラのローカル座標軸ベクトルを取得する
	currentUp = GetUpVector();
	currentRight = GetRightVector();
	currentForward = GetForwardVector();

	//各回転軸からこのフレームでの回転量を表すクォータニオン
	Quaternion yawDelta   = Quaternion::FromAxisAngle(currentUp,      YawAngle);
	Quaternion pitchDelta = Quaternion::FromAxisAngle(currentRight,   PitchAngle);
	Quaternion rollDelta  = Quaternion::FromAxisAngle(currentForward, RollAngle);

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%f\n%f\n%f\n%f\n", pitchDelta.x,pitchDelta.y,pitchDelta.z,pitchDelta.w);
	//3つのQuaternionを合成してこのフレームでの回転を表すQuaternion
	Quaternion deltaRotation = rollDelta * pitchDelta * yawDelta;

	//カメラの向きに計算したQuaternionを適応
	orientation = deltaRotation * orientation;

	//念のため正規化
	orientation.Normalize();

	//-------------ここから移動処理

	//回転を適用した後の軸ベクトルを取得
	currentForward = GetForwardVector();
	currentRight = GetRightVector();
	currentUp = GetUpVector();

	//各方向への入力を保存
	Vector3D inputVec;
	// 各方向のインプットを取得
	if (CheckHitKey(KEY_INPUT_W)) { inputVec.z += 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { inputVec.z -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { inputVec.x += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { inputVec.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_SPACE)) { inputVec.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_LCONTROL)) { inputVec.y -= 1.0f; }

	//√2走法を回避
	inputVec.Normalize();

	//ワールド空間のベクトルを作成
	Vector3D worldMoveOffset;

	//各ローカル軸の方向に入力と移動速度で移動ベクトルを計算
	worldMoveOffset += currentForward * inputVec.z * MOVE_SPEED;
	worldMoveOffset += currentRight * inputVec.x * MOVE_SPEED;
	worldMoveOffset += currentUp * inputVec.y * MOVE_SPEED;

	position += worldMoveOffset;

	SetMousePoint(WINDOW_MID_WIDTH, WINDOW_MID_HEIGHT);
}

Matrix Camera::GetViewMatrix() const
{
	//カメラの回転の共役を取得（オブジェクトはカメラと逆に回る）
	Quaternion invOrientation = orientation.Conjugate();
	//クォータニオンを回転行列に変換
	Matrix invRotationMatrix = invOrientation.ToRotationMatrix();
	//カメラが中心になるように平行移動行列を計算
	Matrix invTranslate = Matrix::Identity();
	invTranslate.m[3][0] = -position.x;
	invTranslate.m[3][1] = -position.y;
	invTranslate.m[3][2] = -position.z;

	//平行移動行列*逆回転行列（この順番を厳守）
	Matrix viewMatrix = MartixMultiply(invTranslate, invRotationMatrix);

	//計算した行列を返す
	return viewMatrix;
}

Matrix Camera::GetProjectionMatrix() const
{
	return Perspective(VERTICOL_VIEW,ASPECT,NEAR_Z,FAR_Z);
}

Vector3D Camera::GetForwardVector() const
{
	// 現在のカメラの向きを行列に変換
	Matrix rotMat = orientation.ToRotationMatrix();
	//ワールド座標での前方ベクトルを回転行列で変換
	return VEC3Transform(FORWARD_VECTOR, rotMat);
}

Vector3D Camera::GetRightVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ローカル座標系の右方ベクトルを回転行列で変換
	return VEC3Transform(RIGHT_VECTOR, rotMat);
}

Vector3D Camera::GetUpVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ローカル座標系の上方ベクトルを回転行列で変換
	return VEC3Transform(UP_VECTOR, rotMat);
}

