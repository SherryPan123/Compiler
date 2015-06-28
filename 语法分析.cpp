#include "data.h"

extern char* err_msg[];


extern vector<comtab> comtabs; //变量表
extern vector<funtab> funtabs; //函数表
extern vector<Quadruple> quadruples;//四元式序列表

extern char token[];
extern bool enter(char name[]);
extern bool isexist(char name[]);
extern bool isFuncExist(char name[]);
extern int FindTypeByName(char tname[]);
extern int FindIdByName(char funname[]);
extern char* GetNameByID(int index);//通过变量id找到名字
extern int GetIdByName(char varname[]);//通过变量名字找到id
extern void BackPatch(int p, int t);
extern int merge(int p1, int p2);

int sno = 1;					//当前四元式序号
int curfunc;					//当前函数序号
int temp = 0;                   //临时变量序号

void program();//1.程序  
void func();//2.过程   +fun
void parameter();//3.参数列表   +var 
void statementlists();//4.语句
void complexStatementlists();//5.复合语句
void varDefinition();//6.变量定义语句  +var
int datatype();//7.数据类型
void inputStatement();//8.输入语句
void outputStatement();//9.输出语句
void assignment();//10.赋值语句
void functioncall();//11.函数调用语句
void passparameter(char []);//12.传递参数
void switchStatement();//13.分支语句
void loopStatement();//14.循环语句
Val expression();//15.表达式  ！！
Val Item();//16.项
Val Factor();//17.因子
TFexit booleaexpression();//18.布尔表达式
TFexit relationexpression();//19.关系表达式
int relation();//20.关系

extern void error(int n);

void advance() {
	lookahead = getToken();
}

bool match(int Tok) {
	bool matched = true;
	if (lookahead != Tok) {
		matched = false;
		error(Tok);
	}
	advance();
	return matched;
}

//1.<程序>—> <过程> [<过程>] 
void program(){
	func();

	while (lookahead == sub){
		func();
	}

	if (lookahead != FEOF) {
		error(unexpectedFileEnd);
	}
}

//2.<过程>—> sub id  <参数列表> <语句> end sub
void func(){
	match(sub);
	
	char funname[MAXIDLEN];
	strcpy(funname, token);
	
	match(id);
	//在表中插入定义的函数名
	if (!enter(funname)){
		strcpy(token, funname);
		error(49);
	}
	//添加函数声明表
	comtabs.back().type = FUN;

	funtabs.push_back(funtab(funname));
	//current function = funtabs.size()-1;
	curfunc = funtabs.size();

	parameter();

	statementlists();

	curfunc = 0;

	match(end);
	
	match(sub);
}

//3.<参数列表>—> ( id as <数据类型> [，id as <数据类型>] ) |ε
void parameter(){
	if (lookahead == LP){
		match(LP);

		char tmpname[MAXIDLEN];
		strcpy(tmpname, token);

		match(id);
		//在表中插入定义的变量
		if (!enter(tmpname)){
			strcpy(token, tmpname);
			error(47);
		}

		match(as);

		int tmptype = datatype();
		comtabs.back().type = tmptype;
		comtabs.back().funid = curfunc;
		funtabs.back().para.push_back(tmptype); //函数表中添加形参类型
		
		while (lookahead == comma){
			
			match(comma);
			
			strcpy(tmpname, token);
			
			match(id);
			//在表中插入定义的变量
			if (!enter(tmpname)){
				strcpy(token, tmpname);
				error(47);
			}

			match(as);

			int tmptype = datatype();
			comtabs.back().type = tmptype;
			comtabs.back().funid = curfunc;
			funtabs.back().para.push_back(tmptype);
		}

		match(RP);

	}
}
/*
4.<语句>—> <分支语句>
| <赋值语句>
| <循环语句>
| <输入语句>
| <输出语句>
| <函数调用语句>
| <变量定义语句>
| <复合语句>
|ε
*/
//**//
void statementlists(){
	if (lookahead == IF){
		//分支语句
		switchStatement();
	}
	else if (lookahead == id){
		//赋值语句
		assignment();
	}
	else if (lookahead == WHILE){
		//while语句
		loopStatement();
	}
	else if (lookahead == input){
		//输入语句
		inputStatement();
	}
	else if (lookahead == output){
		//输出
		outputStatement();
	}
	else if (lookahead == call){
		//函数调用
		functioncall();
	}
	else if (lookahead == var){
		//变量定义
		varDefinition();
	}
	else if (lookahead == begin){
		//复合语句
		complexStatementlists();
	}
}

//5.   <复合语句>—> begin  <语句>  [ ,  <语句> ] end
void complexStatementlists(){
	match(begin);
	
	statementlists();

	while (lookahead == comma){
		match(comma);
		statementlists();
	}

	match(end);
}

//6.<变量定义语句>—> var id [ , id ] as <数据类型> ；
void varDefinition(){
	match(var);
	char varname[MAXIDLEN];
	strcpy(varname, token);

	match(id);

	int cnt = 1;
	if (!enter(varname)){
		strcpy(token, varname);
		error(47);
	}


	while (lookahead == comma){
		match(comma);

		strcpy(varname, token);
		match(id);
		cnt++;
		if (!enter(varname)){
			strcpy(token,varname);
			error(47);
		}
	}
	match(as);

	int typetmp = datatype();
	for (int i = 1; i <= cnt; i++){
		comtabs[comtabs.size() - i].type = typetmp;
	}

	match(semicolon);
}

//7.<数据类型>—> integer | float
int datatype(){
	if (lookahead == INTEGER){
		advance();
		return INTEGER;

	}
	else if (lookahead == FLOAT){
		advance();
		return FLOAT;
	}
	else{
		match(INTEGER);		//出错信息
		return -1;
	}
}

//8.<输入语句>—> input  id  [ , id ] ；
void inputStatement(){
	match(input);

	char varname[MAXIDLEN];
	strcpy(varname, token);
	match(id);
	if (!isexist(varname)){
		strcpy(token, varname);
		error(46);
	}

	while (lookahead == comma){
		match(comma);
		strcpy(varname, token);
		match(id);
		if (!isexist(varname)){
			strcpy(token, varname);
			error(46);
		}
	}
	match(semicolon);
}

//9.<输出语句>—> output  <表达式>  [ ,  <表达式> ] ；
void outputStatement(){
	match(output);
	expression();
	while (lookahead == comma){
		match(comma);
		expression();
	}
	match(semicolon);
}

//10.<赋值语句>—> id  =  <表达式> ；
void assignment(){
	char varname[MAXIDLEN];
	strcpy(varname, token);
	Val v2 = Val(1, GetIdByName(varname));

	match(id);
	if (!isexist(varname)){
		strcpy(token, varname);
		error(46);
	}
	
	match(equl);
	Val v = expression();

	match(semicolon);

	quadruples.push_back(Quadruple(sno++, equl, v, v2));
	//生成四元式
	/*printf("%3d （=，", sno++);
	if (v.type == 1) printf("%s", GetNameByID(v.value1));
	else if (v.type == 0) printf("%d", v.value1);
	else if (v.type == 2) printf("%lf", v.value2);
	else printf("t%d", v.value1);
	printf("，%s）\n", varname);*/
}

//11.<函数调用语句>—> call id (  <传递参数>  ) ；
void functioncall(){
	match(call);
	char funname[MAXIDLEN];
	strcpy(funname, token);

	match(id);  //函数名
	if (!isFuncExist(funname)){//函数是否存在
		strcpy(token, funname);
		error(48);
	}
	
	match(LP);
	passparameter(funname);
	match(RP);
	match(semicolon);
}

//12.<传递参数>—> id [ ,  id ]| ε
void passparameter(char funname[]){
	vector<int> tmptypes;//实参类型序列
	char varname[MAXIDLEN];
	strcpy(varname, token);

	if (lookahead==id){
		match(id);
		if (!isexist(varname)){
			strcpy(token, varname);
			error(46);
		}

		int tmptype = FindTypeByName(varname);

		tmptypes.push_back(tmptype);

		while (lookahead == comma){
			match(comma);

			strcpy(varname, token);
			
			match(id);

			if (!isexist(varname)){
				strcpy(token, varname);
				error(46);
			}

			tmptype = FindTypeByName(varname);
			tmptypes.push_back(tmptype);
		}
		int callfuncId = FindIdByName(funname);
		if (tmptypes.size() != funtabs[callfuncId].para.size()){
			error(51);
		}
		else{
			for (int i = 0; i < tmptypes.size(); i++){
				if (tmptypes[i] != funtabs[callfuncId].para[i]){
					error(52);
				}
			}
		}
	}
}

//13.<分支语句>—> if  <布尔表达式> then  <语句> 
//| if  <布尔表达式> then  <语句> else <语句>
void switchStatement(){
	if (lookahead == IF){
		match(IF);
		TFexit tf = booleaexpression();//真假出口
		match(THEN);
		//填真出口
		BackPatch(tf.TC, sno);

		statementlists();
		
		if (lookahead == ELSE){
			// 1 无条件跳转
			int j = sno;
			quadruples.push_back(Quadruple(sno++, 0,Val(),Val()));
			match(ELSE);
			//回填假出口
			BackPatch(tf.FC, sno);
			statementlists();
			//回填 1 无条件跳转
			BackPatch(j, sno);
		}
		else {
			// 回填假出口
			BackPatch(tf.FC, sno);
		}
	}
	else{
		error(24);
	}
 }

//14.<循环语句>—> while  <布尔表达式> do <语句>
void loopStatement(){
	match(WHILE);
	//int boolId = 序号全局变量
	int boolId = sno;
	TFexit tf = booleaexpression();//真假出口

	match(DO);
	//回填真出口
	BackPatch(tf.TC, sno);

	statementlists();
	//(j boolId)
	quadruples.push_back(Quadruple(sno++, 0, Val(), Val(), boolId));
	//回填假出口
	BackPatch(tf.FC, sno);
}

//15.<表达式>—> <项> [ +|－ <项> ]
Val expression(){
	Val v1 = Item();
	while (lookahead == plus || lookahead == minus){
		int op = lookahead;

		if (lookahead == plus) match(plus);
		else match(minus);
		Val v2 = Item();

		quadruples.push_back(Quadruple(sno++, op == plus ? plus : minus, v1,v2,temp));
		//生成四元式
		/*printf("%3d （%c，", sno++, op == plus ? '+' : '-');

		if (v1.type == 1) printf("%s", GetNameByID(v1.value1));
		else if (v1.type == 0)  printf("%d", v1.value1);
		else if (v1.type == 2) printf("%lf", v1.value2);
		else printf("t%d", v1.value1);
		printf("，");
		if (v2.type == 1) printf("%s", GetNameByID(v2.value1));
		else if (v2.type == 0) printf("%d", v2.value1);
		else if (v2.type == 2) printf("%lf", v2.value2);
		else printf("t%d", v2.value1);
		printf("，t%d）\n", temp);*/

		v1.type = -1;
		v1.value1 = temp++;

	}
	return v1;
}

//16.<项>—> <因子> [ *|/  <因子> ]
Val Item(){
	Val v1 = Factor();

	while (lookahead == mutiply || lookahead == div){
		int op = lookahead;
		if (op == mutiply) match(mutiply);
		else match(div);
		Val v2 = Factor();


		quadruples.push_back(Quadruple(sno++, op == mutiply ? mutiply : div, v1, v2, temp));
		//生成四元式
		/*printf("%3d （%c，", sno++, op == mutiply ? '*' : '/');
		
		if (v1.type == 1) printf("%s", GetNameByID(v1.value1));
		else if (v1.type == 0)  printf("%d", v1.value1);
		else if (v1.type == 2) printf("%lf", v1.value2);
		else printf("t%d", v1.value1);
		printf("，");
		if (v2.type == 1) printf("%s", GetNameByID(v2.value1));
		else if (v2.type == 0) printf("%d", v2.value1);
		else if (v2.type == 2) printf("%lf", v2.value2);
		else printf("t%d", v2.value1);
		printf("，t%d）\n", temp);*/

		v1.type = -1;
		v1.value1 = temp++;

	}
	return v1;
}

//17.<因子>—> id | con | deci | (<表达式>)
Val Factor(){
	Val val;
	if (lookahead == id){
		
		val.type = 1;//变量
		
		char varname[MAXIDLEN];
		strcpy(varname, token);
		match(id);
		if (!isexist(varname)){
			strcpy(token, varname);
			error(46);
		}
		val.value1 = GetIdByName(varname);
	}
	else if (lookahead == con){
		
		val.type = 0;//整数
		val.value1 = atoi(token);

		match(con);
	}
	else if (lookahead == deci){

		val.type = 2;//小数
		val.value2 = atof(token);

		match(deci);
	}
	else if (lookahead == LP){
		match(LP);

		val = expression();
		
		match(RP);

		
	}
	else {
		error(24);
	}
	return val;
}

//18.<布尔表达式>—> <关系表达式> [ and | or  <布尔表达式> ]
TFexit booleaexpression(){
	TFexit reexit = relationexpression();//1 关系表达式
	TFexit tf = reexit;//指向当前真假出口
	while (lookahead == and || lookahead==or){
		if (lookahead == and){
			match(and);
			//关系表达式 1 的真出口确定
			quadruples[reexit.TC].result = sno;

			TFexit tmp = booleaexpression();
			
			//TFexit tmp = relationexpression();

			//quadruples[tmp.TC].result = sno;
			tf.FC = merge(tmp.FC, tf.FC);
			tf.TC = tmp.TC;//唯一的一个真出口
			//合并真/假出口
			return tf;
		}
		else if (lookahead == or){
			match(or);
			//关系表达式 1 的假出口确定
			quadruples[reexit.FC].result = sno;

			TFexit tmp = booleaexpression();

			//TFexit tmp = relationexpression();

			//quadruples[tmp.FC].result = sno;
			tf.TC = merge(tmp.TC, tf.TC);
			tf.FC = tmp.FC;//唯一的一个假出口
			//返回真假出口
			return tf;
		}
	}

	return tf;
}

//19. <关系表达式>—> <表达式> <关系> <表达式>
TFexit relationexpression(){
	Val v1 = expression();
	int re = relation();
	Val v2 = expression();
	quadruples.push_back(Quadruple(sno++, re, v1, v2));
	quadruples.push_back(Quadruple(sno++,0,Val(),Val()));//无条件跳转op=0
	return TFexit(sno - 2, sno - 1);
}

//20. <关系>—> < | <= | > | >= | == | <>
int relation(){
	if (lookahead == lessthan){
		match(lessthan);
		return lessthan;
	}
	else if (lookahead == lessequl){
		match(lessequl);
		return lessequl;
	}
	else if (lookahead == morethan){
		match(morethan);
		return morethan;
	}
	else if (lookahead == moreequl){
		match(moreequl);
		return moreequl;
	}
	else if (lookahead == equlequl){
		match(equlequl);
		return equlequl;
	}
	else if (lookahead == notequl){
		match(notequl);
		return notequl;
	}
	else {
		error(24);
		return ERROR;
	}
}
