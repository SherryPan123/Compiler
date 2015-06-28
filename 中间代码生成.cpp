#include "data.h"

extern vector<Quadruple> quadruples;//��Ԫʽ���б�

int merge(int p1, int p2){//����������Ϊp1\p2���������ϲ���������������
	int p;
	if (!p2) return p1;
	else{
		p = p2;
		while (quadruples[p].result)
			p = quadruples[p].result;
		quadruples[p].result = p1;
		return p2;
	}
}

void BackPatch(int p, int t){ //����Ԫʽ���t������pΪ�׵���
	int q = p;
	while (q){
		int q1 = quadruples[q].result;
		quadruples[q].result = t;
		q = q1;
	}
	return;
}