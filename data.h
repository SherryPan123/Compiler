#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>


using namespace std;


#define FEOF                   -1
#define ERROR                  -2
#define unexpectedFileEnd      23


#define id						1
#define con						2	 //整数
#define plus					3
#define minus					4
#define mutiply					5
#define div						6
#define LP						7	  
#define RP						8    
#define deci					9	 //小数
#define equl					10	 //=
#define lessthan				11   //<
#define morethan				12   //>
#define lessequl				13   //<=
#define moreequl				14   //>=
#define notequl					15   //<>
#define equlequl				16   //==
#define and						17   //bool运算符and
#define or						18   //bool运算符or
#define comma					19   //,
#define semicolon				20	 //;
#define singlenote				21   //单行注释//
#define mulnote					22   //多行注释/**/
#define function				25

//define keyword 保留字
#define begin					30
#define as						31
#define sub						32
#define end						33
#define IF						34
#define THEN					35
#define ELSE					36
#define call					37
#define WHILE					38
#define DO						39
#define INTEGER					40
#define FLOAT					41
#define input					42
#define output					43
#define var						44
	
#define MAXIDLEN 15		//标识符最大长度
#define MAXPARANUM 10	//函数参数最大个数


extern int lookahead;
extern FILE * fin; 
extern int row, col;

extern void advance();
extern int getToken();

struct comtab{			//符号表
	char name[MAXIDLEN + 1];
	bool used;
};


struct Val{					//
	int value1;
	double value2;
	int  type;  // 1：表示value中保存的是标识符在符号表中的索引；
	// 0：表示value中保存的是常量值；
	//-1：表示value中保存的是临时变量序号；
};

struct funtab{		//函数声明表
	char name[MAXIDLEN + 1]; //函数名
	int paranum;	//参数个数
	int para[MAXPARANUM];//参数类型
	int returnval;//返回值类型
};