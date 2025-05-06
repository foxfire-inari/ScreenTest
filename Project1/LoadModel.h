#pragma once
#include"Singleton.h"
#include<fstream>

class LoadModel : public Singleton<LoadModel>
{
public:
	LoadModel();;

	FILE* file = fopen("","");
};

