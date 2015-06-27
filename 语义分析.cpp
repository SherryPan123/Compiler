#include "data.h"

extern vector<comtab> comtabs; //������
extern vector<funtab> funtabs; //������
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

bool isFuncExist(char name[]){
	for (int i = 0; i < comtabs.size(); i++){

		if (strcmp(comtabs[i].name, name) == 0)
			return true;

	}
	return false;
}

bool istypematch(char name[],int type){
	for (int i = 0; i < comtabs.size(); i++){

		if (strcmp(comtabs[i].name, name) == 0 && comtabs[i].funid == curfunc && comtabs[i].type == type)
			return true;

	}
	return false;
}

int FindTypeByName(char tname[]){
	for (int i = 0; i < comtabs.size(); i++){
		if (strcmp(comtabs[i].name, tname) == 0 && comtabs[i].funid == curfunc)
			return comtabs[i].type;
	}
}

int FindIdByName(char funname[]){
	for (int i = 0; i < funtabs.size(); i++){
		if (strcmp(funtabs[i].name,funname)==0) {
			return i;
		}
	}
}