#include "data.h"

extern vector<Quadruple> quadruples;//��Ԫʽ���б�
extern char* GetNameByID(int index);//ͨ������id�ҵ�����

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

char operation(int op){
	if (op == plus) return '+';
	else if (op == minus) return '-';
	else if (op == mutiply) return '*';
	else if (op == div) return '/';
	else if (op == equl) return '=';
}
/*
lessthan
morethan
lessequl
moreequl
notequl
equlequl
*/
char* joperation(int op){
	if (op == lessthan) return "<";
	else if (op == morethan) return ">";
	else if (op == lessequl) return "<=";
	else if (op == moreequl) return ">=";
	else if (op == notequl) return "<>";
	else return "=";
}

char* getValue(Val a){
	char *x = new char[MAXIDLEN];
	if (a.type == 0){
		sprintf(x, "%d", a.value1);
		return x;
	}
	else if (a.type == 2){
		sprintf(x, "%.2lf", a.value2);
		return  x;
	}
	else if (a.type == 1) return GetNameByID(a.value1);
	else if (a.type == -1){
		sprintf(x, "t%d", a.value1);
		return x;
	}
}

void PrintQuadruple(Quadruple q){//���һ����Ԫʽ
	printf("%3d ", q.ID);
	if (q.op == 0) {//��������ת
		printf("(j ,    ,    ,%2d)\n",q.result);
		return;
	}
	if (q.op > 10 && q.op < 17){//��������ת
		printf("(j%s,%3s,%3s,%d)\n", joperation(q.op), getValue(q.arg1), getValue(q.arg2), q.result);
		return;
	}
	printf("(%c , ", operation(q.op));
	if (q.op == equl){
		printf("%3s,%3s)\n",getValue(q.arg1), getValue(q.arg2));
		return;
	}
	printf("%3s,%3s,t%d)\n", getValue(q.arg1), getValue(q.arg2), q.result);
	return;

}