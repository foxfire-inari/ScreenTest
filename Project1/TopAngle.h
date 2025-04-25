#pragma once
#include"Common.h"
#include"Vector.h"

class Camera;
/// <summary>
/// 3D��Ԃ������낷�N���X
/// </summary>
class TopAngle
{
public:
	TopAngle(Camera* _camera);
	~TopAngle();
	void Draw(std::vector< std::vector<Vector3D>> _worldpos);
private:
	Camera* camera;
};

