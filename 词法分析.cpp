#include "data.h"

extern FILE * fin;
extern char token[256];
extern int col, row;
int prerow, precol;  //����ǰ���кš��к�


//�ؼ��ֱ�
extern char Keyword[20][10];
//bool�����
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

int _getToken()//�ʷ���������
{
	int tokeType = FEOF;         //�ʷ��������صĵ�������
	char ch = getnext();

	if (ch != EOF)
	{
		while (ch == ' ' || ch == '\n' || ch == '\t'){
			if (ch == '\n'){
				col = 0;
				row++;
			}
			ch = getnext();  //�������еķָ���
		}

		if (ch == EOF) return FEOF;  //�ļ�����

		if (isalpha(ch))   //�������ĸ������б�ʶ������
		{
			int j = 0;
			token[j++] = ch;
			ch = getnext();
			while (isalpha(ch) || isalnum(ch))  //�������ĸ�����ֻ�$����ϱ�ʶ��������������ʶ����Ͻ���
			{
				token[j++] = ch; //��ϵı�ʶ��������token��
				ch = getnext();  //����һ���ַ�
			}
			token[j] = '\0';     //��ʶ����Ͻ���

			tokeType = iskeyword(token);

			if (tokeType == -2){
				tokeType = isboolop(token);
				if (tokeType == -2) tokeType = id;
			}

			if (ch != EOF){
				fseek(fin, -1, 1);   //����һ���ַ� 
				col--;
			}
		}
		else if (isalnum(ch) && ch != '0')	//���ִ��� ��������С��,����ĸ��Ϊ0
		{
			int numberofdecimal = 0;  //С�������
			int j = 0;
			token[j++] = ch;
			ch = getnext();  //����һ���ַ�
			if (ch == '.') numberofdecimal++;
			while ((isalnum(ch) || ch == '.') && numberofdecimal < 2)  //�����������������������������������Ͻ���
			{
				token[j++] = ch;  //�������/С��������token��
				ch = getnext();  //����һ���ַ�
				if (ch == '.') numberofdecimal++;
			}
			token[j] = '\0';  //����/С����Ͻ���   

			if (numberofdecimal) tokeType = deci;  //��һ��С���������һ���������
			else if (numberofdecimal == 0) tokeType = con;

			if (ch != EOF){
				fseek(fin, -1, 1);   //����һ���ַ� 
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
					fseek(fin, -1, 1);   //����һ���ַ� 
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
					fseek(fin, -1, 1);   //����һ���ַ� 
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
					fseek(fin, -1, 1);   //����һ���ַ� 
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
					fseek(fin, -1, 1);   //����һ���ַ� 
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
