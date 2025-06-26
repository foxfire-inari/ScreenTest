#pragma once
#include <string>
#include "Singleton.h"
#include "Vector.h"

struct FaceObject;
/// <summary>
/// objファイルを読み込むシングルトンクラス
/// </summary>
class ObjFile : public Singleton<ObjFile>
{
public:
	ObjFile();
	~ObjFile();
	/// <summary>
	/// .objファイルから線分情報を取得
	/// </summary>
	/// <param name="fileName">開くファイル(文字列を操作することは無いのでstringではない)</param>
	/// <returns>線分情報</returns>
	FaceObject LoadModel(const char* fileName);
private:
	/// <summary>
	/// getlineした文字列をdelimiterごとに分割
	/// </summary>
	/// <param name="input">gerlineした文字列</param>
	/// <param name="delimiter">分割する文字</param>
	/// <returns></returns>
	std::vector<std::string>split(std::string& input, char delimiter);

};

