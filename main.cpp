#include "data.h"

//�ؼ��ֱ�
char Keyword[20][10] = { "begin", "as", "sub", "end", "if", "then", "else", "call", "while",
"do", "integer", "float", "input", "output", "var" };

//bool�����
char Boolop[2][5] = { "and", "or" };

//vector<comtab> ;//ÿ�㶼Ϊһ�����ű�


FILE * fin;
int isright;
int lookahead;
char token[256];

int col = 0, row = 1;

extern void advance();
extern void program();

void main()
{
	char Scanin[256] = "c.txt";
	//printf("������Դ�����ļ���������·������");
	//scanf_s("%s", Scanin, 255);
	int errno_terr;
	if (errno_terr = fopen_s(&fin, Scanin, "r"))
	{
		printf("��Դ�����ļ�����!\n");
		return;
	}

	isright = 1;                                 //ʶ���� 
	advance();

	program();

	if (isright == 0 || lookahead != FEOF){
		printf("\n����ʧ�ܣ�\n");

	}
	else{
		printf("\n�����ɹ���\n");
	}

	fclose(fin);

	getchar();
	getchar();
}


//���Դʷ�����
/*
void main(){
	char Scanin[256] = "C:\\Users\\PNT\\Desktop\\Compiler\\Compiler_PNT\\a.txt";
	int errno_terr;
	if (errno_terr = fopen_s(&fin, Scanin, "r"))
	{
		printf("��Դ�����ļ�����!\n");

		return;
	}

	while (true){
		int x= getToken();
		if (x == ERROR){
			//printf("%s ����\n", token);
			continue;
		}
		if (x == FEOF) break;
		printf("%s  %d\n", token, x);
	}
	printf("OK!\n");
	getchar();
}
*/