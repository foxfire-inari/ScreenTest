#include "TopAngle.h"
#include"Camera.h"
#include "DxLib.h"

namespace
{
	//XZïΩñ ÇÃèkè¨î{ó¶
	static const float VIEW_SIZE = 10.f;

	//ècâ°ÇÃï`âÊîÕàÕ
	static const float VIEW_WIDTH  = 100.f;
	static const float VIEW_HEIGHT = 150.f;


}



TopAngle::TopAngle(Camera* _camera)
	:camera{ _camera }
{
}

TopAngle::~TopAngle()
{
}

void TopAngle::Draw(std::vector<std::vector<Vector3D>> _worldpos)
{
	//îwåiï`âÊ
	DrawBox(0, 0, VIEW_WIDTH, VIEW_HEIGHT, GetColor(100, 100, 100), true);
	DrawLine(VIEW_WIDTH, 0, VIEW_WIDTH, VIEW_HEIGHT, GetColor(255, 255, 255), true);
	DrawLine(0, VIEW_HEIGHT, VIEW_WIDTH, VIEW_HEIGHT, GetColor(255, 255, 255), true);

	//ÉJÉÅÉâï`âÊ
	Vector3D campos;
	campos.x = camera->GetPosition().x / VIEW_SIZE + VIEW_WIDTH / 2;
	campos.y = camera->GetPosition().z / VIEW_SIZE;
	campos.z = 0;
	DrawCircle(campos.x, campos.y, 5, GetColor(255, 255, 255));
	DrawLine(campos.x, campos.y, campos.x, campos.y - 50,GetColor(0,0,255));


	//å©Ç¶ÇƒÇ¢ÇÈîÕàÕÇï`âÊ
	Vector3D scrpos = campos;
	scrpos.y = camera->GetScreenPos().z / VIEW_SIZE;
	DrawLine(campos.x, campos.y, scrpos.x + WINDOW_WIDTH / VIEW_SIZE, scrpos.y, GetColor(0, 255, 255), true);
	DrawLine(campos.x, campos.y, scrpos.x - WINDOW_WIDTH / VIEW_SIZE, scrpos.y, GetColor(0, 255, 255), true);
	DrawLine(scrpos.x + WINDOW_WIDTH / VIEW_SIZE, scrpos.y, scrpos.x - WINDOW_WIDTH / VIEW_SIZE, scrpos.y, GetColor(0, 255, 255), true);

	//ï`âÊópÇÃç¿ïWÇéùÇ¬îzóÒ
	std::vector<Vector3D> scpos;

	for (int i = 0; i < _worldpos.size(); i++)
	{
		for (int j = 0; j < _worldpos[i].size(); j++)
		{
			Vector3D viewpos;
			viewpos.x = _worldpos[i][j].x / VIEW_SIZE + VIEW_WIDTH/2;
			viewpos.y = _worldpos[i][j].z / VIEW_SIZE;
			viewpos.z = 0;
			scpos.emplace_back(viewpos);
		}
		//ê¸ï™Çï`âÊ
		DrawLine(scpos[0].x, scpos[0].y, scpos[1].x, scpos[1].y, GetColor(255, 255, 255));

		scpos.clear();
	}
}
