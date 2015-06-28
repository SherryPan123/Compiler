#include "data.h"

extern vector<Quadruple> quadruples;//四元式序列表

int merge(int p1, int p2){//将两条链首为p1\p2的两条链合并，返回新链链首
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

void BackPatch(int p, int t){ //用四元式序号t回填以p为首的链
	int q = p;
	while (q){
		int q1 = quadruples[q].result;
		quadruples[q].result = t;
		q = q1;
	}
	return;
}