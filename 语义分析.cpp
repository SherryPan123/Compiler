#include "data.h"

extern vector<comtab> comtabs; //������
extern int curfunc; //��ǰ����

bool enter(char name[])
{
	for (int i = 0; i < comtabs.size(); i++){

		if (strcmp(comtabs[i].name, name)==0 && comtabs[i].funid==curfunc)
			return false;

	}

	comtabs.push_back(comtab(name,curfunc));
	return true;
}

bool isexist(char name[]){
	for (int i = 0; i < comtabs.size(); i++){

		if (strcmp(comtabs[i].name, name) == 0 && comtabs[i].funid==curfunc)
			return true;

	}
	return false;
}