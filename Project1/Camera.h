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
	//カメラの座標
	Vector3D position;

	//スクリーンの座標
	Vector3D screenPos;


	//カメラの回転(向き)
	Vector3D rotation;

	/// <summary>
	/// スクリーン座標に変換
	/// </summary>
	/// <param name="worldPos">3D空間の座標</param>
	/// <returns>スクリーン座標</returns>
	Vector3D GetScreenPos(Vector3D worldPos);

};

