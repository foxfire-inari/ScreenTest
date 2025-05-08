#include "ObjFile.h"
#include "MeshObject.h"
#include "LineVertex.h"
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

MeshObject ObjFile::LoadModel(const char* fileName)
{
	//ファイルからの入力を得るためにifstreamを使用
	std::ifstream ifs(fileName);
	//全ての処理済みの要素をまとめる変数
	MeshObject result;

	//ファイルが開かなければ何も入れずリターン
	if (!ifs.is_open())return result;

	//一列分の文字列を受け取る変数
	std::string line;


	//頂点情報のみを保存
	std::vector<Vector3D> vSource;
	//uv座標のみを保存
	//std::vector<float[2]> vtSource = {};

	//一時保存用の変数
	Vector3D vec;


	//分割された行を保存
	std::vector<std::string> strvec = {};


	//'/'で分割された頂点のインデックスを保存
	std::vector<std::string> fSource;

	//線分として扱う頂点座標を保存
	LineVertex fPoint;

	while (std::getline(ifs, line))
	{
		//読み取った行を分割
		strvec = split(line, ' ');

		//使わない要素は無視(のちに実装するため覚えやすいようにまとめておく)
		//strvec.at(0) == "#"		//コメント
		//vec.at(0) == "mellib"		//HTLファイルのファイル名
		//vec.at(0) == "usemtl"		//使用マテリアル
		//vec.at(0) == "o"			//オブジェクト
		//vec.at(0) == "vt"			//uv座標
		//vec.at(0) == "vn"			//法線
		//vec.at(0) == "s"			//スムーズシェーディング

		if (strvec.at(0) == "v")//頂点座標を保存
		{
			//順番に取得
			vec.x = stof(strvec.at(1))*10;
			vec.y = stof(strvec.at(2))*10;
			vec.z = stof(strvec.at(3))*10;
			//頂点座標を追加
			vSource.push_back(vec);
			//念のためゼロにする
			vec.Zero();
		}
		else if (strvec.at(0) == "f")//指定の頂点で多角形を作る
		{
			//多角形を形成する頂点情報を保存
			std::vector<int> fVertex = {};

			//二番目以降の要素が欲しい
			for (int i = 1; i < strvec.size(); i++)
			{
				//さらに'/'で分割
				fSource = split(strvec.at(i), '/');

				if (!fSource.empty())
				{
					//頂点情報のみ取得
					fVertex.push_back(stoi(fSource.at(0)));
					fSource.clear();
				}
			}

			//頂点から線分を作る
			for (int i = 0; i < fVertex.size(); i++)
			{
				int startPoint = i;
				int endPoint = i + 1;
				if (endPoint >= fVertex.size()) 
				{ 
					endPoint = 0;
				}
				//線分として扱う座標を取得
				fPoint =
				{
					//fで指定されている数字は1から始まるので-1する
					vSource.at(fVertex.at(startPoint)-1),
					vSource.at(fVertex.at(endPoint)-1)
				};

				//----------------------------ここに既に存在する線分かを判断する式を加える

				result.lineVertexs.push_back(fPoint);
			}
			fVertex.clear();

		}
		//初期化
		strvec.clear();
	}

	return result;
}

std::vector<std::string> ObjFile::split(std::string& input, char delimiter)
{
	std::vector<std::string> result;

	//文字列(input)からの入力を得るためにistringstreamを使用
	std::istringstream stream(input);
	//分割された文字列を受け取る変数
	std::string str;


	//delimiterごとに分割して保存
	while (std::getline(stream, str, delimiter))
	{
		result.emplace_back(str);
	}

	//分割されたものをまとめて返す
	return result;
}
