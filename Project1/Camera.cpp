#include "Camera.h"
#include "DxLib.h"

namespace
{

	//�J�����̈ړ����x
	static const float MOVE_SPEED = 2.5f;

	//�}�E�X���x
	static const float ROTATION_SENSITIVITY = 5.f;                           
	//�␳�t���̉�]��
	static const float ANGLE_RATE = ONE_DEGREE / ROTATION_SENSITIVITY;    

	//�e�����̏���l
	const float MIN_V = -DX_PI_F / 180.0f * 72.5f;
	const float MAX_V = DX_PI_F / 180.0f * 72.5f;
	const float MIN_H = 0.0f;
	const float MAX_H = DX_PI_F * 2.0f;
}


Camera::Camera()
	:position{ 0,0,0 }
	,screenPos{0,0,-SCREEN}
	, rotation{ 0,0,0 }
{
}

Camera::~Camera()
{
}

void Camera::Draw(std::vector< std::vector<Vector3D>> _worldpos)
{
	//�`��p�̍��W�����z��
	std::vector<Vector3D> scpos;
	bool isInScreen[2] = { false,false };

	//DrawFormatString(0, 0, GetColor(255, 255, 255),
	//	"CameraLocation:(%f,%f,%f)", camsc.x, camsc.y, camsc.z);

	for (int i = 0; i < _worldpos.size(); i++)
	{
		isInScreen[0] = _worldpos[i][0].z < position.z;
		isInScreen[1] = _worldpos[i][1].z < position.z;
		if (isInScreen[0] == false && isInScreen[1] == false)continue;

		for (int j = 0; j < _worldpos[i].size(); j++)
		{
			//�X�N���[�����W�ɕϊ����Ēǉ�
			scpos.emplace_back(GetScreenPos(_worldpos[i][j]));
		}

		//������`��
		DrawLine(scpos[0].x, scpos[0].y,scpos[1].x, scpos[1].y,GetColor(255, 255, 255));

		//DrawFormatString(0, 32+32 * i, GetColor(255, 255, 255),
		//	"SLine:(%f,%f) ELine:(%f,%f)",
		//	scpos[0].x, scpos[0].y, scpos[1].x, scpos[1].y);

		scpos.clear();

	}
}

void Camera::Update()
{
	//�}�E�X�̈ړ��ʂ�ۑ�------------------------�}�E�X�֘A�͕ʂŃV���O���g���N���X���쐬����
	int mouseMoveX = 0;
	int mouseMoveY = 0;

	//���݂̃}�E�X�̍��W���擾
	GetMousePoint(&mouseMoveX, &mouseMoveY);

	//�}�E�X���������ʂ��v�Z
	mouseMoveX -= WINDOW_WIDTH / 2;
	mouseMoveY -= WINDOW_HEIGHT / 2;

	//�������ʂ���p�x���v�Z
	rotation.x += mouseMoveY * ANGLE_RATE;
	rotation.y += mouseMoveX * ANGLE_RATE;

	//�J���������������E�������Z�b�g
	if (rotation.x > MAX_V)	rotation.x = MAX_V;
	if (rotation.x < MIN_V)	rotation.x = MIN_V;
	if (rotation.y > MAX_H)	rotation.y -= MAX_H;
	if (rotation.y < MIN_H)	rotation.y += MAX_H;

	if (CheckHitKey(KEY_INPUT_W))position.z += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_S))position.z -= MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_D))position.x += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_A))position.x -= MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_E))position.y += MOVE_SPEED;
	if (CheckHitKey(KEY_INPUT_Q))position.y -= MOVE_SPEED;

	screenPos = position;
	screenPos.z -= SCREEN;

	//�}�E�X�̍��W���Œ�
	SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

}

Vector3D Camera::GetScreenPos(Vector3D _worldPos)
{
	//�J�����̍��W�Ō��Z����
	Vector3D worldPos = _worldPos - position;

	//�X�N���[���܂ł̋��������[���h���W�Ŋ���
	float scDis = SCREEN / worldPos.z;

	//scDis�ŏ�Z���邱�Ƃŉ����̂��̂قǒ����Ɋ��A�������Ȃ�B�i���ߊ����ł�j
	Vector3D result;
	result.x = worldPos.x * scDis + WINDOW_WIDTH / 2;
	result.y = worldPos.y * scDis + WINDOW_HEIGHT / 2;
	result.z = 0.f;

	return result;
}
