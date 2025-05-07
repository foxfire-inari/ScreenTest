#include "ObjFile.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>


ObjFile* ObjFile::Singleton::instance = nullptr;

ObjFile::ObjFile()
{
}

ObjFile::~ObjFile()
{
}

std::vector<std::vector<Vector3D>> ObjFile::LoadModel(const char* fileName)
{
	//ファイルからの入力を得るためにifstreamを使用
	std::ifstream ifs(fileName);
	//全ての処理済みの要素をまとめる変数
	std::vector<std::vector<Vector3D>> result;

	//頂点情報のみを保存
	std::vector<Vector3D> vSource;
	//uv座標のみを保存
	//std::vector<float[2]> vtSource = {};


	//ファイルが開かなければ何も入れずリターン
	if (!ifs.is_open())return result;

	//一列分の文字列を受け取る変数
	std::string line;

	while (std::getline(ifs, line))
	{
		//読み取った行を分割
		std::vector<std::string> strvec = split(line, ' ');

		//使わない要素なら無視(のちに実装するため覚えやすいようにまとめておく)
		if (strvec.at(0) == "#"		 || //コメント
			strvec.at(0) == "mellib" || //HTLファイルのファイル名
			strvec.at(0) == "usemtl" || //使用マテリアル
			strvec.at(0) == "o"		 || //オブジェクト
			strvec.at(0) == "vt"	 || //uv座標
			strvec.at(0) == "vn"	 || //法線
			strvec.at(0) == "s"		    //スムーズシェーディング
			)
		{
			continue;
		}

		//頂点座標を保存
		if (strvec.at(0) == "v")
		{
			Vector3D vec;
			//順番に取得
			vec.x = stof(strvec.at(1));
			vec.y = stof(strvec.at(2));
			vec.z = stof(strvec.at(3));
			//頂点座標を追加
			vSource.push_back(vec);
			//念のためゼロにする
			vec.Zero();
		}

		//指定の頂点で多角形を作る
		if (strvec.at(0) == "f")
		{
			//多角形を形成する頂点情報を保存
			std::vector<int> fVector = {};

			//二番目以降の要素が欲しい
			for (int i = 1; i < strvec.size(); i++)
			{
				//さらに'/'で分割
				std::vector<std::string> fSource = split(strvec.at(i), '/');

				//頂点情報のみ取得
				fVector.push_back(stoi(fSource.at(0)));
				fSource.clear();
			}

			//頂点から線分を作る
			for (int i = 0; i < fVector.size(); i++)
			{
				int startPoint = i;
				int endPoint = i + 1;
				if (endPoint >= fVector.size()) 
				{ 
					endPoint = 0;
				}
				//線分として扱う座標を取得
				std::vector<Vector3D> fPoint =
				{
					//fで指定されている数字は1から始まるので-1する
					vSource.at(fVector.at(startPoint)-1),
					vSource.at(fVector.at(endPoint)-1)
				};
				result.push_back(fPoint);
				fPoint.clear();
			}
			fVector.clear();

		}
		//初期化
		strvec.clear();
	}

	return result;
}

std::vector<std::string> ObjFile::split(std::string& input, char delimiter)
{
	//文字列(input)からの入力を得るためにistringstreamを使用
	std::istringstream stream(input);
	//分割された文字列を受け取る変数
	std::string str;

	//分割された文字列をまとめる変数
	std::vector<std::string> result;

	//delimiterごとに分割して保存
	while (std::getline(stream, str, delimiter))
	{
		result.emplace_back(str);
	}

	//分割されたものをまとめて返す
	return result;
}
