#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"


namespace
{

	/// <summary>
	/// 4D�x�N�g�� start ���� end �ցA�p�����[�^ t (0.0�`1.0) ���g���Đ��`��Ԃ���C�����C���֐� 
	///�i�N���b�s���O�Ō�_���v�Z����ۂɎg�p����j
	/// </summary>
	/// <param name="start">�J�n�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="t">�i�s�x</param>
	/// <returns>4D�x�N�g��</returns>
	inline Vector4D VectorLerp4D(const Vector4D& start, const Vector4D& end, float t)
	{
		return {
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t,
			start.w + (end.w - start.w) * t
		};
	}

	//-----Chohen-Sutherland�A���S���Y��

	//�A�E�g�R�[�h�i�r�b�g���Z���s�����߂̕��j
	const int INSIDE		=  0;	//000000
	const int TOP			=  1;	//000001
	const int BOTTOM		=  2;	//000010
	const int LEFT			=  4;	//000100
	const int RIGHT			=  8;	//001000
	const int OUTCODE_NEAR	= 16;	//010000
	const int OUTCODE_FAR	= 32;	//100000

	/// <summary>
	/// �N���b�v���W�ɂȂ������A�ǂ̗̈�ɂ��邩��߂�A�E�g�R�[�h
	/// </summary>
	/// <param name="p"></param>
	/// <returns></returns>
	int ComputeOutCode(const Vector4D& p)
	{
		//�����Ƃ��ď�����
		int code = INSIDE;

		//W���O��菬�������͂�����O�ɂ��邱�Ƃ������m�肵�Ă���
		//X,Y�̓N���b�s���O������ɂ�����x��
		if (p.w < 0.0f)
		{
			code |= OUTCODE_NEAR;
			return code;
		}

		// �㉺���E�\���Ŕ�r���A�O���ɂ�������Ή�����r�b�g�𗧂Ă�

		//�����ɋN���肦�Ȃ��̂�else�ł܂Ƃ߂�
		if (p.x < -p.w) { code |= LEFT; }
		else if (p.x > p.w) { code |= RIGHT; }

		if (p.y < -p.w) { code |= BOTTOM; }
		else if (p.y > p.w) { code |= TOP; }

		if (p.z < 0.0f) { code |= OUTCODE_NEAR; }
		else if (p.z > p.w) { code |= OUTCODE_FAR; }

		return code;
	}

	bool ClipLineCohenSutherland(Vector4D& startClip, Vector4D& endClip)
	{
		//�n�_�I�_�̃A�E�g�R�[�h���Z�b�g
		int startOutcode	= ComputeOutCode(startClip);
		int endOutcode		= ComputeOutCode(endClip);

		//float�̌덷�ɂ�薳�����[�v���������Ă��܂��̂�h������
		const int MAX_ITERATIONS = 10;


	}


	//�J�����̈ړ����x
	static const float MOVE_SPEED = 2.5f;

	//�}�E�X���x
	static const float ROTATION_SENSITIVITY = 5.f;                           

}


Camera::Camera()
	:position{ 0,0,1000 }
{
}

Camera::~Camera()
{
}

void Camera::Draw(std::vector< std::vector<Vector3D>> _worldpos)
{

}

void Camera::Update()
{

}

Matrix Camera::GetViewMatrix() const
{
	return Matrix();
}

Matrix Camera::GetProjectionMatrix() const
{
	return Matrix();
}

Vector3D Camera::GetForwardVector() const
{
	return Vector3D();
}

Vector3D Camera::GetRightVector() const
{
	return Vector3D();
}

Vector3D Camera::GetUpVector() const
{
	return Vector3D();
}
