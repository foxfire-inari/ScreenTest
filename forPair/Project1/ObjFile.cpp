#include "ObjFile.h"
#include "MeshObject.h"
#include "LineVertex.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>

namespace
{
	//頂点のおおよその最大数
	static const int VERTEX_ELEMENT = 20000;
	//線分のおおよその最大数
	static const int LINE_ELEMENT = 50000;
	//多角形のおおよその最大数
	static const int FACE_INDEX_ELEMENT = 5;
}

ObjFile* ObjFile::Singleton::instance = nullptr;

ObjFile::ObjFile()
{
}

ObjFile::~ObjFile()
{
}

MeshObject ObjFile::LoadModel(const char* fileName)
{
	MeshObject result = MeshObject(LINE_ELEMENT);

	//ファイルからの入力を得るためにifstreamを使用
	std::ifstream ifs(fileName);

	//ファイルが開かなければ何も入れずリターン
	if (!ifs.is_open())return result;


	//頂点情報のみを保存
	std::vector<Vector3D> vertexSource;
	//先にメモリを確保しておく
	vertexSource.reserve(VERTEX_ELEMENT);

	//uv座標のみを保存
	//std::vector<float[2]> vtSource = {};
	
	//線分に使われるインデックスを保存
	std::vector<std::pair<int, int>> lineIndexSource;
	//先にメモリを確保しておく
	lineIndexSource.reserve(LINE_ELEMENT);

	//一列分の文字列を受け取る変数
	std::string line;


	//'/'で分割された頂点のインデックスを文字列で保存
	std::vector<std::string> faceIndexStr;
	//先にメモリを確保しておく
	faceIndexStr.reserve(FACE_INDEX_ELEMENT);

	//一行ずつファイルを読み込む
	while (std::getline(ifs, line))
	{
		if (line == "")continue;
		//読み取った行を分割
		std::vector<std::string> strvec = split(line, ' ');

		//使わない要素は無視(のちに実装するため覚えやすいようにまとめておく)
		//strvec.at(0) == "#"		//コメント
		//vec.at(0) == "mellib"		//HTLファイルのファイル名
		//vec.at(0) == "usemtl"		//使用マテリアル
		//vec.at(0) == "o"			//オブジェクト(頂点、uv座標、法線には関係ない要素なので今回のプロジェクトにおいては無視しても問題ない)
		//vec.at(0) == "vt"			//uv座標
		//vec.at(0) == "vn"			//法線
		//vec.at(0) == "s"			//スムーズシェーディング

		if (strvec.at(0) == "v")//頂点座標を保存
		{
			//一時保存用の変数
			Vector3D vec;

			//順番に取得
			vec.x = stof(strvec.at(1))*100;
			vec.y = stof(strvec.at(2))*100;
			vec.z = stof(strvec.at(3))*100;

			//頂点座標を追加
			vertexSource.push_back(vec);
		}
		else if (strvec.at(0) == "f")//指定の頂点で多角形を作る
		{
			//１つの多角形に使われる頂点のインデックスを保存
			std::vector<int> usedVertexIndex = {};

			//二番目以降の要素が欲しいので1から
			for (int i = 1; i < strvec.size(); ++i)
			{
				//さらに'/'で分割
				faceIndexStr = split(strvec.at(i), '/');

				if (!faceIndexStr.empty())
				{
					//頂点のインデックスのみ取得
					usedVertexIndex.push_back(stoi(faceIndexStr.at(0)));
					//一応クリア
					faceIndexStr.clear();
				}
			}

			//頂点のインデックスから線分を作る
			for (int i = 0; i < usedVertexIndex.size(); ++i)
			{
				//nとn+1番目の頂点で線分を作成
				int startPoint = i;
				int endPoint = i + 1;

				//n+1番目の頂点が最大値を超えたか
				if (endPoint >= usedVertexIndex.size())
				{
					//最初の頂点を取得するようにする
					endPoint = 0;
				}

				//fで指定されている数字は1から始まるので-1する
				int startIndex	= usedVertexIndex.at(startPoint) - 1;
				int endIndex	= usedVertexIndex.at(endPoint) - 1;

				//インデックスが小さい順に並べなおす
				std::pair<int, int> line = (endIndex > startIndex) ? std::make_pair(startIndex, endIndex) : std::make_pair(endIndex, startIndex);

				//既に存在するペアかを取得
				auto it = std::find(lineIndexSource.begin(), lineIndexSource.end(), line);

				//存在しなければ追加
				if (it == lineIndexSource.end())
					lineIndexSource.push_back(line);
			}
			usedVertexIndex.clear();
		}
	}

	//インデックスから線分を作成
	for (int i = 0; i < lineIndexSource.size(); ++i)
	{
		//線分を生成
		LineVertex lineVertex;
		lineVertex.vertexPair = std::make_pair
		(
			vertexSource[lineIndexSource[i].first],
			vertexSource[lineIndexSource[i].second]
		);
		result.lineVertexs.push_back(lineVertex);
	}
	//使用していないメモリを開放
	result.lineVertexs.shrink_to_fit();
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
