#include "data.h"

extern FILE * fin;
extern char token[256];
extern int col, row;
int prerow, precol;  //出错前的行号、列号


//关键字表
extern char Keyword[20][10];
//bool运算符
extern char Boolop[2][5];

bool isalnum(char ch)
{
	return '0' <= ch && ch <= '9';
}

bool isalpha(char ch)
{
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '$';
}

int iskeyword(char ch[]){
	for (int i = 0; i < 20; i++){
		if (strcmp(ch, Keyword[i]) == 0){
			return i + begin;
		}
	}
	return -2;  
}

int isboolop(char ch[]){
	for (int i = 0; i < 2; i++){
		if (strcmp(ch, Boolop[i]) == 0)	return i+and;
	}
	return -2;
}

char getnext(){
	col++;
	return getc(fin);
}

int _getToken()//词法分析函数
{
	int tokeType = FEOF;         //词法分析返回的单词类型
	char ch = getnext();

	if (ch != EOF)
	{
		while (ch == ' ' || ch == '\n' || ch == '\t'){
			if (ch == '\n'){
				col = 0;
				row++;
			}
			ch = getnext();  //跳过所有的分隔符
		}

		if (ch == EOF) return FEOF;  //文件结束

		if (isalpha(ch))   //如果是字母，则进行标识符处理
		{
			int j = 0;
			token[j++] = ch;
			ch = getnext();
			while (isalpha(ch) || isalnum(ch))  //如果是字母或数字或$则组合标识符；如果不是则标识符组合结束
			{
				token[j++] = ch; //组合的标识符保存在token中
				ch = getnext();  //读下一个字符
			}
			token[j] = '\0';     //标识符组合结束

			tokeType = iskeyword(token);

			if (tokeType == -2){
				tokeType = isboolop(token);
				if (tokeType == -2) tokeType = id;
			}

			if (ch != EOF){
				fseek(fin, -1, 1);   //回退一个字符 
				col--;
			}
		}
		else if (isalnum(ch) && ch != '0')	//数字处理 整数或者小数,首字母不为0
		{
			int numberofdecimal = 0;  //小数点个数
			int j = 0;
			token[j++] = ch;
			ch = getnext();  //读下一个字符
			if (ch == '.') numberofdecimal++;
			while ((isalnum(ch) || ch == '.') && numberofdecimal < 2)  //如果是数字则组合整数；如果不是则整数组合结束
			{
				token[j++] = ch;  //组合整数/小数保存在token中
				ch = getnext();  //读下一个字符
				if (ch == '.') numberofdecimal++;
			}
			token[j] = '\0';  //整数/小数组合结束   

			if (numberofdecimal) tokeType = deci;  //有一个小数点和至少一个数字组成
			else if (numberofdecimal == 0) tokeType = con;

			if (ch != EOF){
				fseek(fin, -1, 1);   //回退一个字符 
				col--;
			}
		}
		else if (ch == '+')
		{
			tokeType = plus;
		}
		else if (ch == '-')
		{
			tokeType = minus;
			
		}
		else if (ch == '*')
		{
			tokeType = mutiply;
		}
		else if (ch == '/')
		{
			ch = getnext();
			if (ch == '/'){
				tokeType = singlenote;
				while (ch != '\n' && ch != EOF){
					ch = getnext();
				}
				if (ch == '\n'){
					row++;
					col = 0;
				}
			}
			else if (ch == '*'){
				tokeType = mulnote;
				while (ch != EOF){
					if (ch == '*'){
						ch = getnext();
						if (ch == '/') break;
					}
					ch = getnext();
					if (ch == '\n'){
						row++;
						col = 0;
					}
				}
			}
			else{
				tokeType = div;

				if (ch != EOF){
					fseek(fin, -1, 1);   //回退一个字符 
					col--;
				}
			}

		}
		else if (ch == '=')
		{
			ch = getnext();
			if (ch == '='){
				tokeType = equlequl;
			}
			else{
				tokeType = equl;

				if (ch != EOF){
					fseek(fin, -1, 1);   //回退一个字符 
					col--;
				}
			}
			
		}
		else if (ch == '<')
		{
			ch = getnext();
			if (ch == '='){
				tokeType = lessequl;
			}
			else if (ch == '>'){
				tokeType = notequl;
			}
			else {
				tokeType = lessthan;
				if (ch != EOF){
					fseek(fin, -1, 1);   //回退一个字符 
					col--;
				}
			}
		}
		else if (ch == '>')
		{
			ch = getnext();
			if (ch == '='){
				tokeType = moreequl;
			}
			else{
				tokeType = morethan;

				if (ch != EOF){
					fseek(fin, -1, 1);   //回退一个字符 
					col--;
				}
			}
		}
		else if (ch == '(')
		{
			tokeType = LP;
		}
		else if (ch == ')')
		{
			tokeType = RP;
		}
		else if (ch == ',')
		{
			tokeType = comma;
		}
		else if (ch == ';')
		{
			tokeType = semicolon;
		}
		else
		{
			tokeType = ERROR;
		}
	}
	return tokeType;
}


int getToken(){
	int x;
	do{
		prerow = row;
		precol = col;
		x = _getToken();
	} while (x == singlenote || x == mulnote);
	return x;
}
