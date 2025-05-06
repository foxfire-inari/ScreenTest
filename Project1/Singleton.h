#pragma once

/// <summary>
/// �V���O���g���N���X�̌p����
/// class ������ : public Singleton<������>
/// </summary>
/// <typename name="T">�q�N���X�̖��O</typename>
template<typename T>
class Singleton
{
private:
	static T* instance;

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q���폜���Ă���
	// ���̍�Ƃŕ�������邱�Ƃ�h�����Ƃ��ł���
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	/// <summary>
	/// �|�C���^��Ԃ�
	/// </summary>
	/// <returns>���g�̃|�C���^</returns>
	static T* GetInstance() {
		return instance;
	};

	/// <summary>
	/// ����
	/// </summary>
	static void Create()
	{
		if (instance == nullptr)instance = new T;
	}

	/// <summary>
	/// �폜
	/// </summary>
	static void Destroy()
	{
		if (instance != nullptr)delete instance;
		instance = nullptr;
	}
};