#include "Camera.h"
#include "DxLib.h"
#include "CameraMath.h"


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
	const int INSIDE		=  0;	//000000
	const int TOP			=  1;	//000001
	const int BOTTOM		=  2;	//000010
	const int LEFT			=  4;	//000100
	const int RIGHT			=  8;	//001000
	const int OUTCODE_NEAR	= 16;	//010000
	const int OUTCODE_FAR	= 32;	//100000

	/// <summary>
	/// クリップ座標になった時、どの領域にあるかを閉めるアウトコード
	/// </summary>
	/// <param name="p"></param>
	/// <returns></returns>
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

	bool ClipLineCohenSutherland(Vector4D& startClip, Vector4D& endClip)
	{
		//始点終点のアウトコードをセット
		int startOutcode	= ComputeOutCode(startClip);
		int endOutcode		= ComputeOutCode(endClip);

		//floatの誤差により無限ループが発生してしまうのを防ぐため
		const int MAX_ITERATIONS = 10;


	}


	//カメラの移動速度
	static const float MOVE_SPEED = 2.5f;

	//マウス感度
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
