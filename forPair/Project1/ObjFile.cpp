#include "ObjFile.h"
#include "MeshObject.h"
#include "LineVertex.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>

namespace
{
	//���_�̂����悻�̍ő吔
	static const int VERTEX_ELEMENT = 20000;
	//�����̂����悻�̍ő吔
	static const int LINE_ELEMENT = 50000;
	//���p�`�̂����悻�̍ő吔
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

	//�t�@�C������̓��͂𓾂邽�߂�ifstream���g�p
	std::ifstream ifs(fileName);

	//�t�@�C�����J���Ȃ���Ή������ꂸ���^�[��
	if (!ifs.is_open())return result;


	//���_���݂̂�ۑ�
	std::vector<Vector3D> vertexSource;
	//��Ƀ��������m�ۂ��Ă���
	vertexSource.reserve(VERTEX_ELEMENT);

	//uv���W�݂̂�ۑ�
	//std::vector<float[2]> vtSource = {};
	
	//�����Ɏg����C���f�b�N�X��ۑ�
	std::vector<std::pair<int, int>> lineIndexSource;
	//��Ƀ��������m�ۂ��Ă���
	lineIndexSource.reserve(LINE_ELEMENT);

	//��񕪂̕�������󂯎��ϐ�
	std::string line;


	//'/'�ŕ������ꂽ���_�̃C���f�b�N�X�𕶎���ŕۑ�
	std::vector<std::string> faceIndexStr;
	//��Ƀ��������m�ۂ��Ă���
	faceIndexStr.reserve(FACE_INDEX_ELEMENT);

	//��s���t�@�C����ǂݍ���
	while (std::getline(ifs, line))
	{
		if (line == "")continue;
		//�ǂݎ�����s�𕪊�
		std::vector<std::string> strvec = split(line, ' ');

		//�g��Ȃ��v�f�͖���(�̂��Ɏ������邽�ߊo���₷���悤�ɂ܂Ƃ߂Ă���)
		//strvec.at(0) == "#"		//�R�����g
		//vec.at(0) == "mellib"		//HTL�t�@�C���̃t�@�C����
		//vec.at(0) == "usemtl"		//�g�p�}�e���A��
		//vec.at(0) == "o"			//�I�u�W�F�N�g(���_�Auv���W�A�@���ɂ͊֌W�Ȃ��v�f�Ȃ̂ō���̃v���W�F�N�g�ɂ����Ă͖������Ă����Ȃ�)
		//vec.at(0) == "vt"			//uv���W
		//vec.at(0) == "vn"			//�@��
		//vec.at(0) == "s"			//�X���[�Y�V�F�[�f�B���O

		if (strvec.at(0) == "v")//���_���W��ۑ�
		{
			//�ꎞ�ۑ��p�̕ϐ�
			Vector3D vec;

			//���ԂɎ擾
			vec.x = stof(strvec.at(1))*100;
			vec.y = stof(strvec.at(2))*100;
			vec.z = stof(strvec.at(3))*100;

			//���_���W��ǉ�
			vertexSource.push_back(vec);
		}
		else if (strvec.at(0) == "f")//�w��̒��_�ő��p�`�����
		{
			//�P�̑��p�`�Ɏg���钸�_�̃C���f�b�N�X��ۑ�
			std::vector<int> usedVertexIndex = {};

			//��Ԗڈȍ~�̗v�f���~�����̂�1����
			for (int i = 1; i < strvec.size(); ++i)
			{
				//�����'/'�ŕ���
				faceIndexStr = split(strvec.at(i), '/');

				if (!faceIndexStr.empty())
				{
					//���_�̃C���f�b�N�X�̂ݎ擾
					usedVertexIndex.push_back(stoi(faceIndexStr.at(0)));
					//�ꉞ�N���A
					faceIndexStr.clear();
				}
			}

			//���_�̃C���f�b�N�X������������
			for (int i = 0; i < usedVertexIndex.size(); ++i)
			{
				//n��n+1�Ԗڂ̒��_�Ő������쐬
				int startPoint = i;
				int endPoint = i + 1;

				//n+1�Ԗڂ̒��_���ő�l�𒴂�����
				if (endPoint >= usedVertexIndex.size())
				{
					//�ŏ��̒��_���擾����悤�ɂ���
					endPoint = 0;
				}

				//f�Ŏw�肳��Ă��鐔����1����n�܂�̂�-1����
				int startIndex	= usedVertexIndex.at(startPoint) - 1;
				int endIndex	= usedVertexIndex.at(endPoint) - 1;

				//�C���f�b�N�X�����������ɕ��ׂȂ���
				std::pair<int, int> line = (endIndex > startIndex) ? std::make_pair(startIndex, endIndex) : std::make_pair(endIndex, startIndex);

				//���ɑ��݂���y�A�����擾
				auto it = std::find(lineIndexSource.begin(), lineIndexSource.end(), line);

				//���݂��Ȃ���Βǉ�
				if (it == lineIndexSource.end())
					lineIndexSource.push_back(line);
			}
			usedVertexIndex.clear();
		}
	}

	//�C���f�b�N�X����������쐬
	for (int i = 0; i < lineIndexSource.size(); ++i)
	{
		//�����𐶐�
		LineVertex lineVertex;
		lineVertex.vertexPair = std::make_pair
		(
			vertexSource[lineIndexSource[i].first],
			vertexSource[lineIndexSource[i].second]
		);
		result.lineVertexs.push_back(lineVertex);
	}
	//�g�p���Ă��Ȃ����������J��
	result.lineVertexs.shrink_to_fit();
	return result;
}

std::vector<std::string> ObjFile::split(std::string& input, char delimiter)
{
	std::vector<std::string> result;

	//������(input)����̓��͂𓾂邽�߂�istringstream���g�p
	std::istringstream stream(input);
	//�������ꂽ��������󂯎��ϐ�
	std::string str;

	//delimiter���Ƃɕ������ĕۑ�
	while (std::getline(stream, str, delimiter))
	{
		result.emplace_back(str);
	}

	//�������ꂽ���̂��܂Ƃ߂ĕԂ�
	return result;
}
