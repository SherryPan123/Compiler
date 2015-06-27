#include "data.h"

extern char token[];
extern int prerow, precol;  //����ǰ���кš��к�

char* err_msg[] =
{
	/*  0 */    "",
	/*  1 */    "ȱ�٣���ʶ�������ʶ��ƴд����",
	/*  2 */    "ȱ�٣�������������ƴд����",
	/*  3 */    "ȱ�٣�+����+ƴд����",
	/*  4 */    "ȱ�٣�-����-ƴд����",
	/*  5 */    "ȱ�٣�*����*ƴд����",
	/*  6 */    "ȱ�٣�/����/ƴд����",
	/*  7 */    "ȱ�٣�(����(ƴд����",
	/*  8 */    "ȱ�٣�)����)д����",
	/*  9 */    "ȱ�٣�С������С��ƴд����",
	/* 10 */    "ȱ�٣�=����=ƴд����",
	/* 11 */    "ȱ�٣�<����<ƴд����",
	/* 12 */    "ȱ�٣�>����>ƴд����",
	/* 13 */    "ȱ�٣�<=����<=ƴд����",
	/* 14 */    "ȱ�٣�>=����>=ƴд����",
	/* 15 */    "ȱ�٣�<>����<>ƴд����",
	/* 16 */    "ȱ�٣�==����==ƴд����",
	/* 17 */    "ȱ�٣�and����andƴд����",
	/* 18 */    "ȱ�٣�or����orƴд����",
	/* 19 */    "ȱ�٣�����ƴд����",
	/* 20 */    "ȱ�٣�����ƴд����",
	/* 21 */    "����ע�ʹ���",
	/* 22 */    "����ע�ʹ���",
	/* 23 */    "����β�����ֶ��൥�ʣ�",
	/* 24 */    "�Ƿ����",
	/* 25 */	"",
	/* 26 */	"",
	/* 27 */	"",
	/* 28 */	"",
	/* 29 */	"",
	/* 30 */	"ȱ�٣�begin����beginƴд����",
	/* 31 */	"ȱ�٣�as����asƴд����",
	/* 32 */	"ȱ�٣�sub����subƴд����",
	/* 33 */	"ȱ�٣�end����endƴд����",
	/* 34 */	"ȱ�٣�if����ifƴд����",
	/* 35 */	"ȱ�٣�then����thenƴд����",
	/* 36 */	"ȱ�٣�else����elseƴд����",
	/* 37 */	"ȱ�٣�call����ƴд����",
	/* 38 */	"ȱ�٣�while����whileƴд����",
	/* 39 */	"ȱ�٣�do����doƴд����",
	/* 40 */	"ȱ�٣�integer����integerƴд����",
	/* 41 */	"ȱ�٣�float����floatƴд����",
	/* 42 */	"ȱ�٣�input����inputƴд����",
	/* 43 */	"ȱ�٣�output����outputƴд����",
	/* 44 */	"ȱ�٣�var����varƴд����",
	/* 45 */	"ȱ�٣�do����doƴд����",
	/* 46 */	"����δ����",
	/* 47 */	"�����ظ�����",
	/* 48 */	"����δ����",
	/* 49 */	"�����ظ�����",
	/* 50 */	"�������Ͳ�ƥ��",
	/* 51 */	"����ʵ�����βε���Ŀ��һ��",
	/* 51 */	"����ʵ�����βε��������Ͳ�һ��",
};

void error(int n) {
	if (n > 45 && n < 51) {
		printf("�кţ�%d  �кţ�%d  %s %s\n", prerow, precol, token, err_msg[n]);
	}
	else {
		printf("�кţ�%d  �кţ�%d  %s\n", prerow, precol, err_msg[n]);
	}
	fclose(fin);
	getchar();
	exit(0);
}
