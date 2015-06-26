#include "data.h"

extern char* err_msg[];

extern int prerow, precol;  //出错前的行号、列号

void program();//1.程序  
void func();//2.过程   +fun
void parameter();//3.参数列表   +var 
void statementlists();//4.语句
void complexStatementlists();//5.复合语句
void varDefinition();//6.变量定义语句  +var
void datatype();//7.数据类型
void inputStatement();//8.输入语句
void outputStatement();//9.输出语句
void assignment();//10.赋值语句
void functioncall();//11.函数调用语句
void passparameter();//12.传递参数
void switchStatement();//13.分支语句
void loopStatement();//14.循环语句
void expression();//15.表达式  ！！
void Item();//16.项
void Factor();//17.因子
void booleaexpression();//18.布尔表达式
void relationexpression();//19.关系表达式
void relation();//20.关系


void error(int n) {
	printf("行号：%d  列号：%d  %s\n", prerow,precol,err_msg[n]);
	fclose(fin);
	getchar();
	exit(0);
}

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
	
	match(id);

	parameter();

	statementlists();

	match(end);
	
	match(sub);
}

//3.<参数列表>—> ( id as <数据类型> [，id as <数据类型>] ) |ε
void parameter(){
	if (lookahead == LP){
		match(LP);

		match(id);
		//在表中插入定义的变量
		match(as);

		datatype();

		while (lookahead == comma){
			
			match(comma);

			match(id);

			match(as);

			datatype();
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

	match(id);

	while (lookahead == comma){
		match(comma);

		match(id);
	}
	match(as);

	datatype();

	match(semicolon);
}

//7.<数据类型>—> integer | float
void datatype(){

	if (lookahead == INTEGER){
		
		//此处在表中添加/返回数据类型

		advance();
	}
	else if (lookahead == FLOAT){

		//此处在表中添加/返回数据类型

		advance();
	}
	else{
		match(INTEGER);		//出错信息
	}
}

//8.<输入语句>—> input  id  [ , id ] ；
void inputStatement(){
	match(input);
	match(id);

	while (lookahead == comma){
		match(comma);
		match(id);
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
	match(id);
	match(equl);
	expression();
}

//11.<函数调用语句>—> call id (  <传递参数>  ) ；
void functioncall(){
	match(call);
	match(id);
	match(LP);
	passparameter();
	match(RP);
	match(semicolon);
}

//12.<传递参数>—> id [ ,  id ]| ε
void passparameter(){
	if (match(id)){
		while (lookahead == comma){
			match(comma);
			match(id);
		}
	}
}

//13.<分支语句>—> if  <布尔表达式> then  <语句> 
//| if  <布尔表达式> then  <语句> else <语句>
void switchStatement(){
	if (lookahead == IF){
		match(IF);
		booleaexpression();
		match(THEN);
		statementlists();
		if (lookahead == ELSE){
			match(ELSE);
			statementlists();
		}
	}
	else{
		error(24);
	}
}

//14.<循环语句>—> while  <布尔表达式> do <语句>
void loopStatement(){
	match(WHILE);
	booleaexpression();
	match(DO);
	statementlists();
}

//15.<表达式>—> <项> [ +|－ <项> ]
void expression(){
	Item();
	while (lookahead == plus || lookahead == minus){
		if (lookahead == plus) match(plus);
		else match(minus);
		Item();
	}
}

//16.<项>—> <因子> [ *|/  <因子> ]
void Item(){
	Factor();
	while (lookahead == mutiply || lookahead == div){
		if (lookahead == mutiply) match(mutiply);
		else match(div);
		Factor();
	}
}

//17.<因子>—> id | con | deci | (<表达式>)
void Factor(){
	if (lookahead == id){
		match(id);
	}
	else if (lookahead == con){
		match(con);
	}
	else if (lookahead == deci){
		match(deci);
	}
	else if (lookahead == LP){
		match(LP);

		expression();
		
		match(RP);
	}
	else {
		error(24);
	}
}

//18.<布尔表达式>—> <关系表达式> [ and | or  <布尔表达式> ]
void booleaexpression(){
	relationexpression();
	while (lookahead == and || lookahead==or){
		if (lookahead == and){
			match(and);
			booleaexpression();
		}
		else if (lookahead == or){
			match(or);
			booleaexpression();
		}
	}
}

//19. <关系表达式>—> <表达式> <关系> <表达式>
void relationexpression(){
	expression();
	relation();
	expression();
}

//20. <关系>—> < | <= | > | >= | == | <>
void relation(){
	if (lookahead == lessthan){
		match(lessthan);
	}
	else if (lookahead == lessequl){
		match(lessequl);
	}
	else if (lookahead == morethan){
		match(morethan);
	}
	else if (lookahead == moreequl){
		match(moreequl);
	}
	else if (lookahead == equlequl){
		match(equlequl);
	}
	else if (lookahead == notequl){
		match(notequl);
	}
	else {
		error(24);
	}
}
