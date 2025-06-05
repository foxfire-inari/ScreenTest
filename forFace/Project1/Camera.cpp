#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"
#include "MeshObject.h"
#include <assert.h>

namespace
{

	/// <summary>
	/// 4Dベクトル start から end へ、パラメータ t (0.0～1.0) を使って線形補間するインライン関数 
	///（クリッピングで交点を計算する際に使用する）
	/// </summary>
	/// <param name="start">開始位置</param>
	/// <param name="end">終了位置</param>
	/// <param name="t">進行度</param>
	/// <returns>4Dベクトル</returns>
	inline Vector4D VectorLerp4D(const Vector4D& start, const Vector4D& end, float t)
	{
		return {
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t,
			start.w + (end.w - start.w) * t
		};
	}

	//-----Chohen-Sutherlandアルゴリズム

	//アウトコード（ビット演算を行うための物）
	static const int INSIDE			=  0;	//000000
	static const int LEFT			=  1;	//000001
	static const int RIGHT			=  2;	//000010
	static const int BOTTOM			=  4;	//000100
	static const int TOP			=  8;	//001000
	static const int OUTCODE_NEAR	= 16;	//010000
	static const int OUTCODE_FAR	= 32;	//100000

	/// <summary>
	/// クリップ座標になった時、どの領域にあるかを決めるアウトコード
	/// </summary>
	/// <param name="p">点P</param>
	/// <returns>点Pが存在する空間</returns>
	int ComputeOutCode(const Vector4D& p)
	{
		//内側として初期化
		int code = INSIDE;

		//Wが０より小さい時はｐが手前にあることだけが確定している
		//X,Yはクリッピングした後にもう一度回す
		if (p.w < 0.0f)
		{
			code |= OUTCODE_NEAR;
			return code;
		}

		// 上下左右表裏で比較し、外側にあったら対応するビットを立てる

		//同時に起こりえないのでelseでまとめる
		if (p.x < -p.w) { code |= LEFT; }
		else if (p.x > p.w) { code |= RIGHT; }

		if (p.y < -p.w) { code |= BOTTOM; }
		else if (p.y > p.w) { code |= TOP; }

		if (p.z < 0.0f) { code |= OUTCODE_NEAR; }
		else if (p.z > p.w) { code |= OUTCODE_FAR; }

		return code;
	}

	/// <summary>
	/// Cohen-Sutherlandアルゴリズムを使用したクリッピング関数
	/// </summary>
	/// <param name="startClip">線形変換時の始点</param>
	/// <param name="endClip">線形変換時の終点</param>
	/// <returns>描画されるか</returns>
	bool ClipLineCohenSutherland(Vector4D& startClip, Vector4D& endClip)
	{
		//始点終点のアウトコードをセット
		int startOutcode	= ComputeOutCode(startClip);
		int endOutcode		= ComputeOutCode(endClip);

		//floatの誤差により無限ループが発生してしまうのを防ぐため
		const int MAX_ITERATIONS = 10;
		//現在の反復回数
		int iterations = 0;

		while (iterations < MAX_ITERATIONS)
		{
			iterations++;

			//ビットorをして0なら両端が内側にある
			if ((startOutcode | endOutcode) == 0)
			{
				//線分全体を表示
				return true;
			}
			//両端点が同じ外部領域（右側や下側等）なら表示されない
			else if ((startOutcode & endOutcode) != 0)
			{
				return false;
			}
			//クリッピングが必要な場合
			else
			{
				//外側にある方の点のアウトコードを選択
				int outsideCode = (startOutcode != 0) ? startOutcode : endOutcode;
				
				//矩形との交点が直線startClipendClipのどの範囲にあるか
				//（0.0f<t<1.0f）の場合は線分startClipendClipのどこか
				float t = 0.0f;

				//交点の座標
				Vector4D intersection;
				
				//線分の方向ベクトル
				Vector4D direction = {
					endClip.x - startClip.x,
					endClip.y - startClip.y,
					endClip.z - startClip.z,
					endClip.w - startClip.w
				};
				
				//割り算の分母
				float denominator;

				//outsideCodeに対する境界平面（描画される範囲の境界）との交差tを計算
				if (outsideCode & LEFT)
				{
					//-w<=xを組み替えてx+w<=0にして考える
					denominator = direction.x + direction.w;
					//値が小さい =　" x = -w "にほぼ平行になり、０除算の危険
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					//交点の範囲を計算
					t = (-startClip.x - startClip.w) / denominator;
				}
				else if (outsideCode & RIGHT)
				{
					denominator = direction.x - direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					t = (startClip.w - startClip.x) / denominator;
				}
				else if (outsideCode & BOTTOM)
				{					
					denominator = direction.y + direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }//ここ関数化したい
					t = (-startClip.y - startClip.w) / denominator;
				}
				else if (outsideCode & TOP)
				{
					denominator = direction.y - direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					t = (startClip.w - startClip.y) / denominator;
				}
				else if (outsideCode & OUTCODE_NEAR)
				{
					//ここだけ0z<=0なので処理がわずかに違う
					if (std::fabsf(direction.z) < 1e-6f) { return false; }
					t = -startClip.z / direction.z;
				}
				else if (outsideCode & OUTCODE_FAR)
				{
					denominator = direction.z - direction.w;
					if (std::fabsf(denominator) < 1e-6f) { return false; }
					t = (startClip.w - startClip.z) / denominator;
				}
				//ここまで来たら何らかのバグが起きていることになる
				else { assert(false); }

				//tが線分上の範囲にない場合は計算をしない
				if (t < 0.0f || t>1.0f) { return false; }

				//交点の座標を計算
				intersection = VectorLerp4D(startClip, endClip, t);

				//外部の点を交点に置き換え、その点のoutcodeを計算しなおす
				if (outsideCode == startOutcode)
				{
					startClip = intersection;
					startOutcode = ComputeOutCode(startClip);
				}
				else
				{
					endClip = intersection;
					endOutcode = ComputeOutCode(endClip);
				}
			}
		}
		//最大まで回るのはfloatが悪さした時
		return false;

	}

	//-----Chohen-Sutherlandアルゴリズム終了

	//カメラの移動速度
	static const float MOVE_SPEED = 2.5f;

	//マウス感度
	static const float ROTATION_SENSITIVITY = ONE_DEGREE * 0.1f;

	//垂直方向の視野角
	static const float VERTICOL_VIEW = 60.0f * ONE_DEGREE;
	//アスペクト比
	static const float ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

	//ニアクリップ面までの距離
	static const float NEAR_Z = 0.1f;
	//ファークリップ面までの距離
	static const float FAR_Z = 1000.0f;
}


Camera::Camera()
	:position{ 0,0,-50.0f }
	,orientation()
	,currentForward	{ 0.0f, 0.0f, 1.0f }
	,currentRight	{ 1.0f, 0.0f, 0.0f }
	,currentUp		{ 0.0f, 1.0f, 0.0f }
{
	//カーソルを中央にセット
	SetMousePoint(static_cast<int>(WINDOW_WIDTH / 2), static_cast<int>(WINDOW_HEIGHT) / 2);

}

Camera::~Camera()
{
}

void Camera::Draw(std::list<MeshObject> worldObjects)
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
			//線分の始点と終点をクリップ座標に変換
			Vector4D startClip = VEC4Transform({ line.vertexPair.first.x, line.vertexPair.first.y, line.vertexPair.first.z, 1.0f }, viewProjMatrix);
			Vector4D endClip = VEC4Transform({ line.vertexPair.second.x, line.vertexPair.second.y, line.vertexPair.second.z, 1.0f }, viewProjMatrix);

			//クリッピング処理のため、コピー
			Vector4D startClipped = startClip;
			Vector4D endClipped = endClip;

			// Cohen-Sutherlandアルゴリズムで線分をクリッピング
			if (ClipLineCohenSutherland(startClipped, endClipped))
			{
				//線分が視錐台内に残った時のみ動く

				//NDC座標とスクリーン座標を計算

				//NDC座標用
				Vector3D startNDC, endNDC;

				//パースペクティブ除算の前に０除算防止
				if (fabsf(startClipped.w) > 1e-6f && fabsf(endClipped.w) > 1e-6f)
				{
					startNDC = { startClipped.x / startClipped.w,  startClipped.y / startClipped.w ,  startClipped.z / startClipped.w };
					endNDC = { endClipped.x / endClipped.w,    endClipped.y / endClipped.w ,  endClipped.z / endClipped.w };

					//NDC座標をスクリーン座標（int）に変換
					float heafWidth = WINDOW_WIDTH / 2.0f;
					float heafHeight = WINDOW_HEIGHT / 2.0f;

					int startX = static_cast<int>(startNDC.x * heafWidth + heafWidth);
					int startY = static_cast<int>(-startNDC.y * heafHeight + heafHeight);//描画上Yを逆にする
					int endX = static_cast<int>(endNDC.x * heafWidth + heafWidth);
					int endY = static_cast<int>(-endNDC.y * heafHeight + heafHeight);//描画上Yを逆にする

					//線の描画はDxlibの関数
					DrawLine(startX, startY, endX, endY, GetColor(255, 255, 255));

				}
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

	//画面中央の座標
	const int centerX = static_cast<int>(WINDOW_WIDTH / 2);
	const int centerY = static_cast<int>(WINDOW_HEIGHT / 2);

	//マウスの移動量を保存
	int mouseMoveX = currentMouseX - centerX;
	int mouseMoveY = currentMouseY - centerY;

	//マウスの移動量から回転角度を保存（ラジアン）
	float YawAngle = static_cast<float>(mouseMoveX) * ROTATION_SENSITIVITY;
	float PitchAngle = static_cast<float>(mouseMoveY) * ROTATION_SENSITIVITY;

	float RollAngle = 0.0f;

	if (CheckHitKey(KEY_INPUT_E)) { RollAngle += 2.0f * ONE_DEGREE; }
	if (CheckHitKey(KEY_INPUT_Q)) { RollAngle -= 2.0f * ONE_DEGREE; }

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

	//-------------角度制限を書くならここ

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

	SetMousePoint(centerX, centerY);

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
	return VEC3Transform({ 0.0f, 0.0f, 1.0f }, rotMat);
}

Vector3D Camera::GetRightVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ローカル座標系の右方ベクトルを回転行列で変換
	return VEC3Transform({ 1.0f, 0.0f, 0.0f }, rotMat);
}

Vector3D Camera::GetUpVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ローカル座標系の上方ベクトルを回転行列で変換
	return VEC3Transform({ 0.0f, 1.0f, 0.0f }, rotMat);
}
