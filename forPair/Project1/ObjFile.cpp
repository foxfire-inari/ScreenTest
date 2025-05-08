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
	//�t�@�C������̓��͂𓾂邽�߂�ifstream���g�p
	std::ifstream ifs(fileName);
	//�S�Ă̏����ς݂̗v�f���܂Ƃ߂�ϐ�
	MeshObject result;

	//�t�@�C�����J���Ȃ���Ή������ꂸ���^�[��
	if (!ifs.is_open())return result;

	//��񕪂̕�������󂯎��ϐ�
	std::string line;


	//���_���݂̂�ۑ�
	std::vector<Vector3D> vSource;
	//uv���W�݂̂�ۑ�
	//std::vector<float[2]> vtSource = {};

	//�ꎞ�ۑ��p�̕ϐ�
	Vector3D vec;


	//�������ꂽ�s��ۑ�
	std::vector<std::string> strvec = {};


	//'/'�ŕ������ꂽ���_�̃C���f�b�N�X��ۑ�
	std::vector<std::string> fSource;

	//�����Ƃ��Ĉ������_���W��ۑ�
	LineVertex fPoint;

	while (std::getline(ifs, line))
	{
		//�ǂݎ�����s�𕪊�
		strvec = split(line, ' ');

		//�g��Ȃ��v�f�͖���(�̂��Ɏ������邽�ߊo���₷���悤�ɂ܂Ƃ߂Ă���)
		//strvec.at(0) == "#"		//�R�����g
		//vec.at(0) == "mellib"		//HTL�t�@�C���̃t�@�C����
		//vec.at(0) == "usemtl"		//�g�p�}�e���A��
		//vec.at(0) == "o"			//�I�u�W�F�N�g
		//vec.at(0) == "vt"			//uv���W
		//vec.at(0) == "vn"			//�@��
		//vec.at(0) == "s"			//�X���[�Y�V�F�[�f�B���O

		if (strvec.at(0) == "v")//���_���W��ۑ�
		{
			//���ԂɎ擾
			vec.x = stof(strvec.at(1))*10;
			vec.y = stof(strvec.at(2))*10;
			vec.z = stof(strvec.at(3))*10;
			//���_���W��ǉ�
			vSource.push_back(vec);
			//�O�̂��߃[���ɂ���
			vec.Zero();
		}
		else if (strvec.at(0) == "f")//�w��̒��_�ő��p�`�����
		{
			//���p�`���`�����钸�_����ۑ�
			std::vector<int> fVertex = {};

			//��Ԗڈȍ~�̗v�f���~����
			for (int i = 1; i < strvec.size(); i++)
			{
				//�����'/'�ŕ���
				fSource = split(strvec.at(i), '/');

				if (!fSource.empty())
				{
					//���_���̂ݎ擾
					fVertex.push_back(stoi(fSource.at(0)));
					fSource.clear();
				}
			}

			//���_������������
			for (int i = 0; i < fVertex.size(); i++)
			{
				int startPoint = i;
				int endPoint = i + 1;
				if (endPoint >= fVertex.size()) 
				{ 
					endPoint = 0;
				}
				//�����Ƃ��Ĉ������W���擾
				fPoint =
				{
					//f�Ŏw�肳��Ă��鐔����1����n�܂�̂�-1����
					vSource.at(fVertex.at(startPoint)-1),
					vSource.at(fVertex.at(endPoint)-1)
				};

				//----------------------------�����Ɋ��ɑ��݂���������𔻒f���鎮��������

				result.lineVertexs.push_back(fPoint);
			}
			fVertex.clear();

		}
		//������
		strvec.clear();
	}

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
