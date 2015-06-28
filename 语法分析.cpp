#include "data.h"

extern char* err_msg[];


extern vector<comtab> comtabs; //������
extern vector<funtab> funtabs; //������

extern char token[];
extern bool enter(char name[]);
extern bool isexist(char name[]);
extern bool isFuncExist(char name[]);
extern int FindTypeByName(char tname[]);
extern int FindIdByName(char funname[]);
extern char* GetNameByID(int index);//ͨ������id�ҵ�����
extern int GetIdByName(char varname[]);//ͨ�����������ҵ�id

int curfunc;					//��ǰ�������
int sno = 0;                    //��Ԫʽ���
int temp = 0;                   //��ʱ�������

void program();//1.����  
void func();//2.����   +fun
void parameter();//3.�����б�   +var 
void statementlists();//4.���
void complexStatementlists();//5.�������
void varDefinition();//6.�����������  +var
int datatype();//7.��������
void inputStatement();//8.�������
void outputStatement();//9.������
void assignment();//10.��ֵ���
void functioncall();//11.�����������
void passparameter(char []);//12.���ݲ���
void switchStatement();//13.��֧���
void loopStatement();//14.ѭ�����
Val expression();//15.���ʽ  ����
Val Item();//16.��
Val Factor();//17.����
void booleaexpression();//18.�������ʽ
void relationexpression();//19.��ϵ���ʽ
void relation();//20.��ϵ

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
	
	char funname[MAXIDLEN];
	strcpy(funname, token);
	
	match(id);
	//�ڱ��в��붨��ĺ�����
	if (!enter(funname)){
		strcpy(token, funname);
		error(49);
	}
	//��Ӻ���������
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

//3.<�����б�>��> ( id as <��������> [��id as <��������>] ) |��
void parameter(){
	if (lookahead == LP){
		match(LP);

		char tmpname[MAXIDLEN];
		strcpy(tmpname, token);

		match(id);
		//�ڱ��в��붨��ı���
		if (!enter(tmpname)){
			strcpy(token, tmpname);
			error(47);
		}

		match(as);

		int tmptype = datatype();
		comtabs.back().type = tmptype;
		comtabs.back().funid = curfunc;
		funtabs.back().para.push_back(tmptype); //������������β�����
		
		while (lookahead == comma){
			
			match(comma);
			
			strcpy(tmpname, token);
			
			match(id);
			//�ڱ��в��붨��ı���
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

//7.<��������>��> integer | float
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
		match(INTEGER);		//������Ϣ
		return -1;
	}
}

//8.<�������>��> input  id  [ , id ] ��
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
	char varname[MAXIDLEN];
	strcpy(varname, token);

	match(id);
	if (!isexist(varname)){
		strcpy(token, varname);
		error(46);
	}
	
	match(equl);
	Val v = expression();

	match(semicolon);

	//������Ԫʽ
	printf("%3d ��=��", sno++);
	if (v.type == 1) printf("%s", GetNameByID(v.value1));
	else if (v.type == 0) printf("%d", v.value1);
	else if (v.type == 2) printf("%lf", v.value2);
	else printf("t%d", v.value1);
	printf("��%s��\n", varname);
}

//11.<�����������>��> call id (  <���ݲ���>  ) ��
void functioncall(){
	match(call);
	char funname[MAXIDLEN];
	strcpy(funname, token);

	match(id);  //������
	if (!isFuncExist(funname)){//�����Ƿ����
		strcpy(token, funname);
		error(48);
	}
	
	match(LP);
	passparameter(funname);
	match(RP);
	match(semicolon);
}

//12.<���ݲ���>��> id [ ,  id ]| ��
void passparameter(char funname[]){
	vector<int> tmptypes;//ʵ����������
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

//13.<��֧���>��> if  <�������ʽ> then  <���> 
//| if  <�������ʽ> then  <���> else <���>
void switchStatement(){
	if (lookahead == IF){
		match(IF);
		booleaexpression();//��ٳ���
		match(THEN);
		//�������
		statementlists();
		
		if (lookahead == ELSE){
			// 1 ��������ת
			match(ELSE);
			//��ٳ���
			statementlists();
			//���� 1 ��������ת
		}
		else {
			// ����ٳ���
		}
	}
	else{
		error(24);
	}
}

//14.<ѭ�����>��> while  <�������ʽ> do <���>
void loopStatement(){
	match(WHILE);
	//int boolId = ���ȫ�ֱ���
	booleaexpression();//��ٳ���
	match(DO);
	//���������
	statementlists();
	//(j boolId)
	//����ٳ���
}

//15.<���ʽ>��> <��> [ +|�� <��> ]
Val expression(){
	Val v1 = Item();
	while (lookahead == plus || lookahead == minus){
		int op = lookahead;

		if (lookahead == plus) match(plus);
		else match(minus);
		Val v2 = Item();

		//������Ԫʽ
		printf("%3d ��%c��", sno++, op == plus ? '+' : '-');

		if (v1.type == 1) printf("%s", GetNameByID(v1.value1));
		else if (v1.type == 0)  printf("%d", v1.value1);
		else if (v1.type == 2) printf("%lf", v1.value2);
		else printf("t%d", v1.value1);
		printf("��");
		if (v2.type == 1) printf("%s", GetNameByID(v2.value1));
		else if (v2.type == 0) printf("%d", v2.value1);
		else if (v2.type == 2) printf("%lf", v2.value2);
		else printf("t%d", v2.value1);
		printf("��t%d��\n", temp);

		v1.type = -1;
		v1.value1 = temp++;

	}
	return v1;
}

//16.<��>��> <����> [ *|/  <����> ]
Val Item(){
	Val v1 = Factor();

	while (lookahead == mutiply || lookahead == div){
		int op = lookahead;
		if (op == mutiply) match(mutiply);
		else match(div);
		Val v2 = Factor();

		//������Ԫʽ
		printf("%3d ��%c��", sno++, op == mutiply ? '*' : '/');
		
		if (v1.type == 1) printf("%s", GetNameByID(v1.value1));
		else if (v1.type == 0)  printf("%d", v1.value1);
		else if (v1.type == 2) printf("%lf", v1.value2);
		else printf("t%d", v1.value1);
		printf("��");
		if (v2.type == 1) printf("%s", GetNameByID(v2.value1));
		else if (v2.type == 0) printf("%d", v2.value1);
		else if (v2.type == 2) printf("%lf", v2.value2);
		else printf("t%d", v2.value1);
		printf("��t%d��\n", temp);

		v1.type = -1;
		v1.value1 = temp++;

	}
	return v1;
}

//17.<����>��> id | con | deci | (<���ʽ>)
Val Factor(){
	Val val;
	if (lookahead == id){
		
		val.type = 1;//����
		
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
		
		val.type = 0;//����
		val.value1 = atoi(token);

		match(con);
	}
	else if (lookahead == deci){

		val.type = 2;//С��
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

//18.<�������ʽ>��> <��ϵ���ʽ> [ and | or  <�������ʽ> ]
void booleaexpression(){
	relationexpression();//1 ��ϵ���ʽ
	while (lookahead == and || lookahead==or){
		if (lookahead == and){
			match(and);
			//��ϵ���ʽ 1 �� �����������ʽ �ļٳ�����ͬ
			//��ϵ���ʽ 1 �������ȷ��
			booleaexpression();
			//������ٳ���
		}
		else if (lookahead == or){
			match(or);
			//��ϵ���ʽ 1 �� �����������ʽ ���������ͬ
			booleaexpression();
			//������ٳ���
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
