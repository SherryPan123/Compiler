#include "data.h"

//关键字表
char Keyword[20][10] = { "begin", "as", "sub", "end", "if", "then", "else", "call", "while",
"do", "integer", "float", "input", "output", "var" };

//bool运算符
char Boolop[2][5] = { "and", "or" };

//vector<comtab> ;//每层都为一个符号表


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
	//printf("请输入源程序文件名（包括路径）：");
	//scanf_s("%s", Scanin, 255);
	int errno_terr;
	if (errno_terr = fopen_s(&fin, Scanin, "r"))
	{
		printf("打开源程序文件出错!\n");
		return;
	}

	isright = 1;                                 //识别结果 
	advance();

	program();

	if (isright == 0 || lookahead != FEOF){
		printf("\n分析失败！\n");

	}
	else{
		printf("\n分析成功！\n");
	}

	fclose(fin);

	getchar();
	getchar();
}


//测试词法分析
/*
void main(){
	char Scanin[256] = "C:\\Users\\PNT\\Desktop\\Compiler\\Compiler_PNT\\a.txt";
	int errno_terr;
	if (errno_terr = fopen_s(&fin, Scanin, "r"))
	{
		printf("打开源程序文件出错!\n");

		return;
	}

	while (true){
		int x= getToken();
		if (x == ERROR){
			//printf("%s 错误\n", token);
			continue;
		}
		if (x == FEOF) break;
		printf("%s  %d\n", token, x);
	}
	printf("OK!\n");
	getchar();
}
*/