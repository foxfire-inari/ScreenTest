#pragma once
#include <string>
#include "Singleton.h"
#include "Vector.h"

struct FaceObject;
/// <summary>
/// obj�t�@�C����ǂݍ��ރV���O���g���N���X
/// </summary>
class ObjFile : public Singleton<ObjFile>
{
public:
	ObjFile();
	~ObjFile();
	/// <summary>
	/// .obj�t�@�C��������������擾
	/// </summary>
	/// <param name="fileName">�J���t�@�C��(������𑀍삷�邱�Ƃ͖����̂�string�ł͂Ȃ�)</param>
	/// <returns>�������</returns>
	FaceObject LoadModel(const char* fileName);
private:
	/// <summary>
	/// getline�����������delimiter���Ƃɕ���
	/// </summary>
	/// <param name="input">gerline����������</param>
	/// <param name="delimiter">�������镶��</param>
	/// <returns></returns>
	std::vector<std::string>split(std::string& input, char delimiter);

};

