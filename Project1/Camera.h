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
	/// ビュー行列を取得する
	///（ワールド座標からカメラ座標への変換用行列）
	/// </summary>
	/// <returns>ビュー行列</returns>
	Matrix GetViewMatrix() const;

	/// <summary>
	/// プロジェクション行列を取得する
	///（透視投影を行う行列）
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	Matrix GetProjectionMatrix() const;


	/// <summary>
	/// カメラの正面のベクトルをワールド座標に変換
	/// </summary>
	/// <returns>カメラの正面（ワールド座標）</returns>
	Vector3D GetForwardVector()const;
	
	/// <summary>
	/// カメラの右のベクトルをワールド座標に変換
	/// </summary>
	/// <returns>カメラの右（ワールド座標）</returns>
	Vector3D GetRightVector()const;
	
	/// <summary>
	/// カメラの上のベクトルをワールド座標に変換
	/// </summary>
	/// <returns>カメラの上（ワールド座標）</returns>
	Vector3D GetUpVector()const;

private:
	//カメラのワールド座標
	Vector3D position;
	//カメラの回転(向き)
	Quaternion orientation;

	//垂直方向の視野角
	float viewVer = 60.0f * ONE_DEGREE;
	//アスペクト比
	float aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
	//ニアクリップ面までの距離
	float nearZ = 0.1f;
	//ファークリップ面までの距離
	float farZ = 1000.0f;

	// 現在のカメラの前方ベクトル (初期値はワールドZ+)
	Vector3D currentForward = { 0.0f, 0.0f, 1.0f };
	// 現在のカメラの右方ベクトル (初期値はワールドX+)
	Vector3D currentRight = { 1.0f, 0.0f, 0.0f };
	// 現在のカメラの上方ベクトル (初期値はワールドY+)
	Vector3D currentUp = { 0.0f, 1.0f, 0.0f };

};

