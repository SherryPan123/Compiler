#include "data.h"

extern char* err_msg[];

extern int prerow, precol;  //����ǰ���кš��к�

void program();//1.����  
void func();//2.����   +fun
void parameter();//3.�����б�   +var 
void statementlists();//4.���
void complexStatementlists();//5.�������
void varDefinition();//6.�����������  +var
void datatype();//7.��������
void inputStatement();//8.�������
void outputStatement();//9.������
void assignment();//10.��ֵ���
void functioncall();//11.�����������
void passparameter();//12.���ݲ���
void switchStatement();//13.��֧���
void loopStatement();//14.ѭ�����
void expression();//15.���ʽ  ����
void Item();//16.��
void Factor();//17.����
void booleaexpression();//18.�������ʽ
void relationexpression();//19.��ϵ���ʽ
void relation();//20.��ϵ


void error(int n) {
	printf("�кţ�%d  �кţ�%d  %s\n", prerow,precol,err_msg[n]);
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

//1.<����>��> <����> [<����>] 
void program(){
	func();

	while (lookahead == sub){
		func();
	}

	if (lookahead != FEOF) {
		error(unexpectedFileEnd);
	}
}

//2.<����>��> sub id  <�����б�> <���> end sub
void func(){
	match(sub);
	
	match(id);

	parameter();

	statementlists();

	match(end);
	
	match(sub);
}

//3.<�����б�>��> ( id as <��������> [��id as <��������>] ) |��
void parameter(){
	if (lookahead == LP){
		match(LP);

		match(id);
		//�ڱ��в��붨��ı���
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
4.<���>��> <��֧���>
| <��ֵ���>
| <ѭ�����>
| <�������>
| <������>
| <�����������>
| <�����������>
| <�������>
|��
*/
//**//
void statementlists(){
	if (lookahead == IF){
		//��֧���
		switchStatement();
	}
	else if (lookahead == id){
		//��ֵ���
		assignment();
	}
	else if (lookahead == WHILE){
		//while���
		loopStatement();
	}
	else if (lookahead == input){
		//�������
		inputStatement();
	}
	else if (lookahead == output){
		//���
		outputStatement();
	}
	else if (lookahead == call){
		//��������
		functioncall();
	}
	else if (lookahead == var){
		//��������
		varDefinition();
	}
	else if (lookahead == begin){
		//�������
		complexStatementlists();
	}
}

//5.   <�������>��> begin  <���>  [ ,  <���> ] end
void complexStatementlists(){
	match(begin);
	
	statementlists();

	while (lookahead == comma){
		match(comma);
		statementlists();
	}

	match(end);
}

//6.<�����������>��> var id [ , id ] as <��������> ��
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

//7.<��������>��> integer | float
void datatype(){

	if (lookahead == INTEGER){
		
		//�˴��ڱ������/������������

		advance();
	}
	else if (lookahead == FLOAT){

		//�˴��ڱ������/������������

		advance();
	}
	else{
		match(INTEGER);		//������Ϣ
	}
}

//8.<�������>��> input  id  [ , id ] ��
void inputStatement(){
	match(input);
	match(id);

	while (lookahead == comma){
		match(comma);
		match(id);
	}
	match(semicolon);
}

//9.<������>��> output  <���ʽ>  [ ,  <���ʽ> ] ��
void outputStatement(){
	match(output);
	expression();
	while (lookahead == comma){
		match(comma);
		expression();
	}
	match(semicolon);
}

//10.<��ֵ���>��> id  =  <���ʽ> ��
void assignment(){
	match(id);
	match(equl);
	expression();
}

//11.<�����������>��> call id (  <���ݲ���>  ) ��
void functioncall(){
	match(call);
	match(id);
	match(LP);
	passparameter();
	match(RP);
	match(semicolon);
}

//12.<���ݲ���>��> id [ ,  id ]| ��
void passparameter(){
	if (match(id)){
		while (lookahead == comma){
			match(comma);
			match(id);
		}
	}
}

//13.<��֧���>��> if  <�������ʽ> then  <���> 
//| if  <�������ʽ> then  <���> else <���>
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

//14.<ѭ�����>��> while  <�������ʽ> do <���>
void loopStatement(){
	match(WHILE);
	booleaexpression();
	match(DO);
	statementlists();
}

//15.<���ʽ>��> <��> [ +|�� <��> ]
void expression(){
	Item();
	while (lookahead == plus || lookahead == minus){
		if (lookahead == plus) match(plus);
		else match(minus);
		Item();
	}
}

//16.<��>��> <����> [ *|/  <����> ]
void Item(){
	Factor();
	while (lookahead == mutiply || lookahead == div){
		if (lookahead == mutiply) match(mutiply);
		else match(div);
		Factor();
	}
}

//17.<����>��> id | con | deci | (<���ʽ>)
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

//18.<�������ʽ>��> <��ϵ���ʽ> [ and | or  <�������ʽ> ]
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

//19. <��ϵ���ʽ>��> <���ʽ> <��ϵ> <���ʽ>
void relationexpression(){
	expression();
	relation();
	expression();
}

//20. <��ϵ>��> < | <= | > | >= | == | <>
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
