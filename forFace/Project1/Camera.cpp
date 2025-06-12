#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"
#include "MeshObject.h"
#include <assert.h>

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

	//Sutherland-Hodgman�A���S���Y��-----------

	/// <summary>
	/// �͂ݏo�Ă������
	/// </summary>
	enum OutVec
	{
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,
		OUTVEC_FAR,
		OUTVEC_NEAR
	};

	Vector4D intersect(const Vector4D& startClip, const Vector4D& endClip, OutVec outvec)
	{
		//�N���b�v��̍��W
		Vector4D result;

		//��`�Ƃ̌�_������startClipendClip�̂ǂ͈̔͂ɂ��邩
		//�i0.0f<t<1.0f�j�̏ꍇ�͐���startClipendClip�̂ǂ���
		float t = 0.0f;

		//�����̕����x�N�g��
		Vector4D direction = {
			endClip.x - startClip.x,
			endClip.y - startClip.y,
			endClip.z - startClip.z,
			endClip.w - startClip.w
		};
		//����Z�̕���
		float denominator;

		switch (outvec)
		{
		case LEFT:
			denominator = direction.x + direction.w;
			if (std::fabsf(denominator) < 1e-6f) { return false; }
			t = (-startClip.x - startClip.w) / denominator;
			break;
		case RIGHT:
			denominator = direction.x - direction.w;
			if (std::fabsf(denominator) < 1e-6f) { return false; }
			t = (startClip.x - startClip.w) / denominator;
			break;
		case BOTTOM:
			denominator = direction.y + direction.w;
			if (std::fabsf(denominator) < 1e-6f) { return false; }//�����֐���������
			t = (-startClip.y - startClip.w) / denominator;
			break;
		case TOP:
			denominator = direction.y - direction.w;
			if (std::fabsf(denominator) < 1e-6f) { return false; }
			t = (startClip.w - startClip.y) / denominator;
			break;
		case OUTVEC_NEAR:
			//��������0z<=0�Ȃ̂ŏ������킸���ɈႤ
			if (std::fabsf(direction.z) < 1e-6f) { return false; }
			t = -startClip.z / direction.z;
			break;
		case OUTVEC_FAR:
			denominator = direction.z - direction.w;
			if (std::fabsf(denominator) < 1e-6f) { return false; }
			t = (startClip.w - startClip.z) / denominator;
			break;
		default:
			assert(false);
		}

		//t��������͈̔͂ɂȂ��ꍇ�͌v�Z�����Ȃ�
		if (t < 0.0f || t>1.0f) { return false; }

		//��_�̍��W���v�Z
		result = VectorLerp4D(startClip, endClip, t);

		return result;

	}


	/// <summary>
	/// �����ɂ��邩
	/// </summary>
	/// <param name="p">���_</param>
	/// <param name="outvec">�m�F�����</param>
	/// <returns>�͂ݏo�Ă��邩�ǂ���</returns>
	bool inside(const Vector4D& p, OutVec outvec)
	{
		switch (outvec)
		{
		case LEFT:			return p.x < -p.w;
		case RIGHT:			return p.x >  p.w;
		case BOTTOM:		return p.y < -p.w;
		case TOP:			return p.y >  p.w;
		case OUTVEC_NEAR:	return p.z < -p.w;
		case OUTVEC_FAR:	return p.z >  p.w;
		}
		return false;
	}


	/// <summary>
	/// �t�F�[�X���N���b�v���A�V�����t�F�[�X�����
	/// </summary>
	/// <param name="face">�t�F�[�X</param>
	/// <param name="outvec">�ΏۂƂ���N���b�v��</param>
	/// <returns>�N���b�v��̃t�F�[�X</returns>
	std::vector<Vector4D> clipEdge(const std::vector<Vector4D>& face, OutVec outvec)
	{
		std::vector<Vector4D> result;

		//�t�F�[�X�̕ӂ�����v�Z
		for (int i = 0; i < face.size(); ++i)
		{
			Vector4D startvertex = face[i];
			Vector4D endvertex = face[(i + 1) % face.size()];

			bool startIn = inside(startvertex, outvec);
			bool endIn   = inside(endvertex, outvec);

			if (startIn && endIn)
			{
				result.push_back(endvertex);
			}
			else if (startIn && !endIn)
			{
				result.push_back(intersect(startvertex, endvertex, outvec));
			}
			else if (!startIn && endIn)
			{
				result.push_back(intersect(startvertex, endvertex, outvec));
				result.push_back(endvertex);
			}
			return result;
		}

	}


	/// <summary>
	/// �t�F�[�X���N���b�v�͈͂Ɏ��߂鏈��
	/// </summary>
	/// <param name="face">�t�F�[�X</param>
	/// <returns>�N���b�v��̃t�F�[�X</returns>
	std::vector<Vector4D> sutherlandHodgmanClip(const std::vector<Vector4D>& face)
	{
		std::vector<Vector4D> result = face;

		//�e�N���b�v�ʂŐ���������
		for (OutVec e : {LEFT, RIGHT, BOTTOM, TOP, OUTVEC_FAR, OUTVEC_NEAR})
		{
			result = clipEdge(result, e);
		}
		return result;


	}

	FaceVertex ChangeNDC(std::vector<Vector4D> face)
	{
		//---------------���͂�������
	}




	std::vector<FaceVertex> Triangulate(const FaceVertex& face)
	{
		std::vector<FaceVertex> result;

		if (face.vertexs.size() < 3)return result;

		for (int i = 1; i + 1 < face.vertexs.size(); ++i)
		{
			FaceVertex triangle = { { face.vertexs[0],face.vertexs[i],face.vertexs[i + 1] },face.normalVer };
			result.push_back(triangle);
		}
	}


	//Sutherland-Hodgman�A���S���Y���I��-------

	//�J�����̈ړ����x
	static const float MOVE_SPEED = 2.5f;

	//�}�E�X���x
	static const float ROTATION_SENSITIVITY = ONE_DEGREE * 0.1f;

	//���������̎���p
	static const float VERTICOL_VIEW = 60.0f * ONE_DEGREE;
	//�A�X�y�N�g��
	static const float ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

	//�j�A�N���b�v�ʂ܂ł̋���
	static const float NEAR_Z = 0.1f;
	//�t�@�[�N���b�v�ʂ܂ł̋���
	static const float FAR_Z = 1000.0f;
}


Camera::Camera()
	:position{ 0,0,-50.0f }
	,orientation()
	,currentForward	{ 0.0f, 0.0f, 1.0f }
	,currentRight	{ 1.0f, 0.0f, 0.0f }
	,currentUp		{ 0.0f, 1.0f, 0.0f }
{
	//�J�[�\���𒆉��ɃZ�b�g
	SetMousePoint(static_cast<int>(WINDOW_WIDTH / 2), static_cast<int>(WINDOW_HEIGHT) / 2);

}

Camera::~Camera()
{
}

void Camera::Draw(std::list<MeshObject> worldObjects)
{
	//�K�v�ȍs������O�Ɍv�Z
	Matrix viewMatrix = GetViewMatrix();
	Matrix projMatrix = GetProjectionMatrix();
	Matrix viewProjMatrix = MartixMultiply(viewMatrix, projMatrix);

	//�I�u�W�F�N�g���ƂɎ擾
	for (const auto& object : worldObjects)
	{
		//�I�u�W�F�N�g�̖ʂ���ʂ��擾
		for (const auto& face : object.faceVertexs)
		{
			std::vector<Vector4D> TransformedFace = TransformFace(face,viewProjMatrix);

			std::vector<Vector4D> ClipedFace = sutherlandHodgmanClip(TransformedFace);

			FaceVertex NDCFace = ChangeNDC(ClipedFace);
			
			std::vector<FaceVertex> triangles = Triangulate(NDCFace);

			// Cohen-Sutherland�A���S���Y���Ő������N���b�s���O
			//if (ClipLineCohenSutherland(startClipped, endClipped))
			//{
			//	//��������������Ɏc�������̂ݓ���
			//
			//	//NDC���W�ƃX�N���[�����W���v�Z
			//
			//	//NDC���W�p
			//	Vector3D startNDC, endNDC;
			//
			//	//�p�[�X�y�N�e�B�u���Z�̑O�ɂO���Z�h�~
			//	if (fabsf(startClipped.w) > 1e-6f && fabsf(endClipped.w) > 1e-6f)
			//	{
			//		startNDC = { startClipped.x / startClipped.w,  startClipped.y / startClipped.w ,  startClipped.z / startClipped.w };
			//		endNDC = { endClipped.x / endClipped.w,    endClipped.y / endClipped.w ,  endClipped.z / endClipped.w };
			//
			//		//NDC���W���X�N���[�����W�iint�j�ɕϊ�
			//		float heafWidth = WINDOW_WIDTH / 2.0f;
			//		float heafHeight = WINDOW_HEIGHT / 2.0f;
			//
			//		int startX = static_cast<int>(startNDC.x * heafWidth + heafWidth);
			//		int startY = static_cast<int>(-startNDC.y * heafHeight + heafHeight);//�`���Y���t�ɂ���
			//		int endX = static_cast<int>(endNDC.x * heafWidth + heafWidth);
			//		int endY = static_cast<int>(-endNDC.y * heafHeight + heafHeight);//�`���Y���t�ɂ���
			//
			//		//���̕`���Dxlib�̊֐�
			//		DrawLine(startX, startY, endX, endY, GetColor(255, 255, 255));
			//
			//	}
			//}
		}
	}
}

void Camera::Update()
{
	//�}�E�X�̌��ݍ��W��ۑ�
	int currentMouseX = 0;
	int currentMouseY = 0;
	GetMousePoint(&currentMouseX, &currentMouseY);

	//��ʒ����̍��W
	const int centerX = static_cast<int>(WINDOW_WIDTH / 2);
	const int centerY = static_cast<int>(WINDOW_HEIGHT / 2);

	//�}�E�X�̈ړ��ʂ�ۑ�
	int mouseMoveX = currentMouseX - centerX;
	int mouseMoveY = currentMouseY - centerY;

	//�}�E�X�̈ړ��ʂ����]�p�x��ۑ��i���W�A���j
	float YawAngle = static_cast<float>(mouseMoveX) * ROTATION_SENSITIVITY;
	float PitchAngle = static_cast<float>(mouseMoveY) * ROTATION_SENSITIVITY;

	float RollAngle = 0.0f;

	if (CheckHitKey(KEY_INPUT_E)) { RollAngle += 2.0f * ONE_DEGREE; }
	if (CheckHitKey(KEY_INPUT_Q)) { RollAngle -= 2.0f * ONE_DEGREE; }

	//�J�����̃��[�J�����W���x�N�g�����擾����
	currentUp = GetUpVector();
	currentRight = GetRightVector();
	currentForward = GetForwardVector();

	//�e��]�����炱�̃t���[���ł̉�]�ʂ�\���N�H�[�^�j�I��
	Quaternion yawDelta   = Quaternion::FromAxisAngle(currentUp,      YawAngle);
	Quaternion pitchDelta = Quaternion::FromAxisAngle(currentRight,   PitchAngle);
	Quaternion rollDelta  = Quaternion::FromAxisAngle(currentForward, RollAngle);

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%f\n%f\n%f\n%f\n", pitchDelta.x,pitchDelta.y,pitchDelta.z,pitchDelta.w);
	//3��Quaternion���������Ă��̃t���[���ł̉�]��\��Quaternion
	Quaternion deltaRotation = rollDelta * pitchDelta * yawDelta;

	//�J�����̌����Ɍv�Z����Quaternion��K��
	orientation = deltaRotation * orientation;

	//�O�̂��ߐ��K��
	orientation.Normalize();

	//-------------�p�x�����������Ȃ炱��

	//-------------��������ړ�����

	//��]��K�p������̎��x�N�g�����擾
	currentForward = GetForwardVector();
	currentRight = GetRightVector();
	currentUp = GetUpVector();

	//�e�����ւ̓��͂�ۑ�
	Vector3D inputVec;
	// �e�����̃C���v�b�g���擾
	if (CheckHitKey(KEY_INPUT_W)) { inputVec.z += 1.0f; }
	if (CheckHitKey(KEY_INPUT_S)) { inputVec.z -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_D)) { inputVec.x += 1.0f; }
	if (CheckHitKey(KEY_INPUT_A)) { inputVec.x -= 1.0f; }
	if (CheckHitKey(KEY_INPUT_SPACE)) { inputVec.y += 1.0f; }
	if (CheckHitKey(KEY_INPUT_LCONTROL)) { inputVec.y -= 1.0f; }

	//��2���@�����
	inputVec.Normalize();

	//���[���h��Ԃ̃x�N�g�����쐬
	Vector3D worldMoveOffset;

	//�e���[�J�����̕����ɓ��͂ƈړ����x�ňړ��x�N�g�����v�Z
	worldMoveOffset += currentForward * inputVec.z * MOVE_SPEED;
	worldMoveOffset += currentRight * inputVec.x * MOVE_SPEED;
	worldMoveOffset += currentUp * inputVec.y * MOVE_SPEED;

	position += worldMoveOffset;

	SetMousePoint(centerX, centerY);

}

Matrix Camera::GetViewMatrix() const
{
	//�J�����̉�]�̋������擾�i�I�u�W�F�N�g�̓J�����Ƌt�ɉ��j
	Quaternion invOrientation = orientation.Conjugate();
	//�N�H�[�^�j�I������]�s��ɕϊ�
	Matrix invRotationMatrix = invOrientation.ToRotationMatrix();
	//�J���������S�ɂȂ�悤�ɕ��s�ړ��s����v�Z
	Matrix invTranslate = Matrix::Identity();
	invTranslate.m[3][0] = -position.x;
	invTranslate.m[3][1] = -position.y;
	invTranslate.m[3][2] = -position.z;

	//���s�ړ��s��*�t��]�s��i���̏��Ԃ�����j
	Matrix viewMatrix = MartixMultiply(invTranslate, invRotationMatrix);

	//�v�Z�����s���Ԃ�
	return viewMatrix;
}

Matrix Camera::GetProjectionMatrix() const
{
	return Perspective(VERTICOL_VIEW,ASPECT,NEAR_Z,FAR_Z);
}

Vector3D Camera::GetForwardVector() const
{
	// ���݂̃J�����̌������s��ɕϊ�
	Matrix rotMat = orientation.ToRotationMatrix();
	//���[���h���W�ł̑O���x�N�g������]�s��ŕϊ�
	return VEC3Transform({ 0.0f, 0.0f, 1.0f }, rotMat);
}

Vector3D Camera::GetRightVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ���[�J�����W�n�̉E���x�N�g������]�s��ŕϊ�
	return VEC3Transform({ 1.0f, 0.0f, 0.0f }, rotMat);
}

Vector3D Camera::GetUpVector() const
{
	Matrix rotMat = orientation.ToRotationMatrix();
	// ���[�J�����W�n�̏���x�N�g������]�s��ŕϊ�
	return VEC3Transform({ 0.0f, 1.0f, 0.0f }, rotMat);
}
