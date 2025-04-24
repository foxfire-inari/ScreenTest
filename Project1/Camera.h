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
	//カメラの座標
	Vector3D location;

	//--------------------------------------カメラの回転を実装


	/// <summary>
	/// スクリーン座標に変換
	/// </summary>
	/// <param name="worldPos">3D空間の座標</param>
	/// <returns>スクリーン座標</returns>
	Vector3D GetScreenPos(Vector3D worldPos);

};

