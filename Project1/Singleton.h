#pragma once

/// <summary>
/// シングルトンクラスの継承元
/// class ●●● : public Singleton<●●●>
/// </summary>
/// <typename name="T">子クラスの名前</typename>
template<typename T>
class Singleton
{
private:
	static T* instance;

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	// コピーコンストラクタと代入演算子を削除しておく
	// この作業で複製されることを防ぐことができる
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	/// <summary>
	/// ポインタを返す
	/// </summary>
	/// <returns>自身のポインタ</returns>
	static T* GetInstance() {
		return instance;
	};

	/// <summary>
	/// 生成
	/// </summary>
	static void Create()
	{
		if (instance == nullptr)instance = new T;
	}

	/// <summary>
	/// 削除
	/// </summary>
	static void Destroy()
	{
		if (instance != nullptr)delete instance;
		instance = nullptr;
	}
};