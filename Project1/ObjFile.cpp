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
	//�t�@�C������̓��͂𓾂邽�߂�ifstream���g�p
	std::ifstream ifs(fileName);
	//�S�Ă̏����ς݂̗v�f���܂Ƃ߂�ϐ�
	std::vector<std::vector<Vector3D>> result;

	//���_���݂̂�ۑ�
	std::vector<Vector3D> vSource;
	//uv���W�݂̂�ۑ�
	//std::vector<float[2]> vtSource = {};


	//�t�@�C�����J���Ȃ���Ή������ꂸ���^�[��
	if (!ifs.is_open())return result;

	//��񕪂̕�������󂯎��ϐ�
	std::string line;

	while (std::getline(ifs, line))
	{
		//�ǂݎ�����s�𕪊�
		std::vector<std::string> strvec = split(line, ' ');

		//�g��Ȃ��v�f�Ȃ疳��(�̂��Ɏ������邽�ߊo���₷���悤�ɂ܂Ƃ߂Ă���)
		if (strvec.at(0) == "#"		 || //�R�����g
			strvec.at(0) == "mellib" || //HTL�t�@�C���̃t�@�C����
			strvec.at(0) == "usemtl" || //�g�p�}�e���A��
			strvec.at(0) == "o"		 || //�I�u�W�F�N�g
			strvec.at(0) == "vt"	 || //uv���W
			strvec.at(0) == "vn"	 || //�@��
			strvec.at(0) == "s"		    //�X���[�Y�V�F�[�f�B���O
			)
		{
			continue;
		}

		//���_���W��ۑ�
		if (strvec.at(0) == "v")
		{
			Vector3D vec;
			//���ԂɎ擾
			vec.x = stof(strvec.at(1));
			vec.y = stof(strvec.at(2));
			vec.z = stof(strvec.at(3));
			//���_���W��ǉ�
			vSource.push_back(vec);
			//�O�̂��߃[���ɂ���
			vec.Zero();
		}

		//�w��̒��_�ő��p�`�����
		if (strvec.at(0) == "f")
		{
			//���p�`���`�����钸�_����ۑ�
			std::vector<int> fVector = {};

			//��Ԗڈȍ~�̗v�f���~����
			for (int i = 1; i < strvec.size(); i++)
			{
				//�����'/'�ŕ���
				std::vector<std::string> fSource = split(strvec.at(i), '/');

				//���_���̂ݎ擾
				fVector.push_back(stoi(fSource.at(0)));
				fSource.clear();
			}

			//���_������������
			for (int i = 0; i < fVector.size(); i++)
			{
				int startPoint = i;
				int endPoint = i + 1;
				if (endPoint >= fVector.size()) 
				{ 
					endPoint = 0;
				}
				//�����Ƃ��Ĉ������W���擾
				std::vector<Vector3D> fPoint =
				{
					//f�Ŏw�肳��Ă��鐔����1����n�܂�̂�-1����
					vSource.at(fVector.at(startPoint)-1),
					vSource.at(fVector.at(endPoint)-1)
				};
				result.push_back(fPoint);
				fPoint.clear();
			}
			fVector.clear();

		}
		//������
		strvec.clear();
	}

	return result;
}

std::vector<std::string> ObjFile::split(std::string& input, char delimiter)
{
	//������(input)����̓��͂𓾂邽�߂�istringstream���g�p
	std::istringstream stream(input);
	//�������ꂽ��������󂯎��ϐ�
	std::string str;

	//�������ꂽ��������܂Ƃ߂�ϐ�
	std::vector<std::string> result;

	//delimiter���Ƃɕ������ĕۑ�
	while (std::getline(stream, str, delimiter))
	{
		result.emplace_back(str);
	}

	//�������ꂽ���̂��܂Ƃ߂ĕԂ�
	return result;
}
