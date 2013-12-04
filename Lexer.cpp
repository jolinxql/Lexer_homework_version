// Lexer.cpp :
//����ʱ�ܽ������ⱻ�����ĳ��򣬲����ʷ������Ľ�����������Ϊ"ѧ��_token.txt"�Ľ���ļ���

#include "stdafx.h"

using namespace std;
string sym[]={"IDEN","CONSTTK","INTTK","CHARTK","VARTK","ARRAYTK","OFTK","IFTK","THENTK","ELSETK","DOTK","WHILETK","FORTK","TOTK","BYTK","DOWNTOTK","PROCETK","FUNCTK","READTK","WRITETK","CALLTK","BEGINTK","ENDTK","PLUS","MINU","MULT","DIV","LSS","LEQ","GRE","GEQ","EQL","NEQ","ASSIGN","SEMICN","COMMA","PERIOD","COLON","QMARK","DQMARK","LPARENT","RPARENT","LBRACK","RBRACK","LBRACE","RBRACE","INTCON","CHARCON","STRCON"};//���������
string reservesym[]={"const","integer","char","var","array","of","if","then","else","do","while","for","to","by","downto","procedure","function","read","write","call","begin","end","+","-","*","/","<","<=",">",">=","=","<>",":=",";",",",".",":","\'","\"","(",")","[","]","{","}"};
enum symbol{IDEN,CONSTTK,INTTK,CHARTK,VARTK,ARRAYTK,OFTK,IFTK,THENTK,ELSETK,DOTK,WHILETK,FORTK,TOTK,BYTK,DOWNTOTK,PROCETK,FUNCTK,READTK,WRITETK,CALLTK,BEGINTK,ENDTK,PLUS,MINU,MULT,DIV,LSS,LEQ,GRE,GEQ,EQL,NEQ,ASSIGN,SEMICN,COMMA,PERIOD,COLON,QMARK,DQMARK,LPARENT,RPARENT,LBRACK,RBRACK,LBRACE,RBRACE,INTCON,CHARCON,STRCON};//ö�ٷ��������

char l,c,br;//c����һ����ȡ���ַ���l����һ����br����debug
int symclass;
string token;
int pt;
char line[LMAX]={0};//һ�ζ�ȡһ��


void mygets(FILE *in){ //���ļ��е��ж�ȡ
	char* cin;
	int linept=0,i;

	cin=fgets(line,LMAX,in);
	i=strlen(line);
	if(!cin){line[i]=-1;line[i+1]='\0'; }
	/*cin=fgetc(in);
	for(i=0;i<1000;i++) //�ַ�������
		line[i]='\0';
	while(cin!='\n'&&cin!=EOF){
		line[linept++]=cin;
		cin=fgetc(in);
	}
	if(cin=='\n')
	{	line[linept++]='\n';
		line[linept]='\0';
	}
	else  //EOF
		line[linept]=-1;
	*/
}
bool isLetter()
{return (c>='a'&&c<='z')||(c>'A'&&c<'Z');}//�ж���ĸ
bool isDigit()
{return c>='0'&&c<='9';}//�ж�����
int reserve(){    //�����Ƿ�Ϊ������
  int i=0;
  for(;i<22;i++){
    if(token==reservesym[i])  return i+1;
  }
  return 0;
}
void lex_error(int errcode){
	printf("\n");
	string errsym;
	if(errcode>=0&&errcode<49){
		errsym=sym[errcode];
		printf("error! symbol class is %s\n",errsym.c_str());
	}
	else
		switch(errcode){
			case ERREND:printf("Lex_Err. Unexpect Ending. You might miss a '.'\n"); break;
			case ERRWORD:printf("Lex_Err. Unexpect Word.\n"); break;
	}
	
	printf("press any key to exit...");
	getchar();

	exit(1);
}
void getsym(){
  token="";
  c=line[++pt];//��ȡһ���ַ�
  //cout<<c;cin>>br;return;
  while(c==' '||c=='\t'||c=='\n') {c=line[++pt];}//�����հ׷��ٶ�ȡһ���ַ���lʼ�ձ�����һ���ַ�
  //cin>>br;return;
  if(c=='\0'||c==-1) return;
  if(isLetter()){                     //�жϵ�ǰ�ַ��Ƿ�Ϊ��ĸ
    //cout<<c<<" is a letter"<<endl;
    //cin>>br;
    while(isLetter()||isDigit()){   //���ַ�ƴ�ӳ��ַ���
      token=token+c;
      c=line[++pt];
      //cout<<"token is"<<token<<endl;cin>>br;return;
    }
    c=line[--pt];    //����һ���ַ�
    int resultValue=reserve();
    if(resultValue==0){
      symclass=IDEN;
    }
    else
      symclass=resultValue;
  }
  else if(isDigit())
  {
    while(isDigit())
    {
      token=token+c;//���ַ�ƴ�ӳ�����
      c=line[++pt];
    }
    c=line[--pt];    //����һ���ַ�
    //ע�⣺��д�﷨����ʱ��Ҫ���ַ���to���ε�ת��������int num=atoi(token.c_str());
    symclass=INTCON;
  }
  else if(c==':')
  { 
    c=line[++pt];
    if(c=='=') symclass=ASSIGN;
    else {c=line[--pt];symclass=COLON;}
  }
  else if(c=='<'){
    c=line[++pt];
    if(c=='=') symclass=LEQ;
    else if(c=='>') symclass=NEQ;
    else {c=line[--pt];symclass=LSS;}
  } 
  else if(c=='>'){
    c=line[++pt];
    if(c=='=') symclass=GEQ;
    else {c=line[--pt];symclass=GRE;}
  } 
  else if(c=='\''){		//�ַ�'a'
	c=line[++pt];
	if(isLetter()||isDigit()){
		if(line[pt+1]=='\''){
			//\' ���ַ�����������ʹ�á����ַ�����(��˫������)����Ҫ��ֻҪ�� ' ���ɡ�
			token=c;
			c=line[++pt];
			symclass=CHARCON;
		}
	}
	else{
		lex_error(CHARCON);			//error����Ҫ�о�
		symclass=QMARK;
	}
  }
  else if(c=='\"'){
	c=line[++pt];
	token="";
	while(c!='\"'){
		if(c=='\0'){
			lex_error(STRCON);break;}
		else{
			if(c=='\\'&&(line[pt+1]=='\\'||line[pt+1]=='\'')){		//\' ���ַ�����������ʹ�á����ַ�����(��˫������)����Ҫ��ֻҪ�� ' ���ɡ�
				c=line[++pt];
				token=token+c;
				c=line[++pt];
			}
			else if((c<127&&c>31)&&(c!=34)){		//���ַ����������ASCII����
				token=token+c;
				c=line[++pt];
			}
			else{
				lex_error(STRCON);}	//�Ƿ��ַ�
		}
	}
	symclass=STRCON;
  }
  else if(c=='=') symclass=EQL;
  else if(c=='+') symclass=PLUS;
  else if(c=='-') symclass=MINU;
  else if(c=='*') symclass=MULT;
  else if(c=='(') symclass=LPARENT;
  else if(c==')') symclass=RPARENT;
  else if(c==',') symclass=COMMA;
  else if(c==';') symclass=SEMICN;
  else if(c=='/') symclass=DIV;
  else if(c=='\"') symclass=DQMARK;
  else if(c=='[') symclass=LBRACK;
  else if(c==']') symclass=RBRACK;
  else if(c=='{') symclass=LBRACE;
  else if(c=='}') symclass=RBRACE;
  else if(c=='.') symclass=PERIOD;
  else lex_error(ERRWORD);//�������Ϸ��ַ�}
  if(symclass>22&&symclass<46) token=reservesym[symclass-1];//���ǵ��ʵ�ʱ��Ҫ����ű�����token��ֵ
}

class word
{
  public:
    static int ct;//������
    string name;//��������
  public:
    word(){};
    void set_name(string name)//���캯����ÿ�δ�ֵ����������һ
    {
      this->name=token;
      ct++;
    }
    void prt_word(FILE *out){   //���ļ����������
      fprintf(out,"%d",ct);//��ӡ���
      string info=" "+sym[symclass]+" "+name+'\n';
      fprintf(out,info.c_str());//�� ����� �������ֻ��з���
    }
};

int word::ct=0;

void main(){
/*/////////////////use for DEBUG & TEST////////////////////////
	FILE *test,*testout;
	char testinput[500]={0};
	char cin;
	char testline[1000]={0};
	int testpt;
	printf("please enter the source file name...\n");
	gets(testinput);
	testout=fopen("c://testout.txt","at");
	test= fopen(testinput,"r");
	while((cin=fgetc(test))!=EOF)
	{	testpt=0;
		do{
			testline[testpt++]=cin;
			cin=fgetc(test);
		}while(cin!='\n'&&cin!=EOF);
		testline[testpt++]='\n';
		testline[testpt]='\0';
		fprintf(testout,"%s",testline);
		if(cin==EOF) break;
	}
	fclose(test);
	fclose(testout);
	return;
//////////////////use for DEBUG & TEST//////////////////////*/
    
	char outfilepath[]="11231001_token.txt";
	char infilepath[500]={0};
	int flag=0;//��Ϊ�쳣�ı�־ֱ����ת��=1�쳣
    FILE *fin,*fout;
    word nextsym;
	fout= fopen(outfilepath,"w");//���ļ�����д������"at"�Ϳ���ʵ�֡���������ھʹ��������ڽ�β׷�ӡ�
	
	//printf("Using default input path...\n>>>Z:\\share\\sae6depart\\6ϵ����\\���뼼��\\����ҵ\\2011���뼼������\\2011�����뼼������\\��ҵ\\�ʷ�����\\lexer\\in.txt\n");
    //strcpy(infilepath,"Z:\\share\\sae6depart\\6ϵ����\\���뼼��\\����ҵ\\2011���뼼������\\2011�����뼼������\\��ҵ\\�ʷ�����\\lexer\\in.txt");

	printf("please enter the source file name...\n");
	gets(infilepath);//�õ�������ļ�·��
	
	fin=fopen(infilepath,"r");
    while(!fin){//��ʧ�������¶����ļ�·��
		printf("No Such File Exist, please check the path and try again...\n");
		gets(infilepath);
		fin=fopen(infilepath,"r");
	}
    while(c!='.'&&c!=-1){
      mygets(fin);pt=-1;
      do{
        getsym();
		if(c==-1)	lex_error(ERREND);
        if(c!=0)
        { nextsym.set_name(token);
          nextsym.prt_word(fout);
        }
        if(c=='.')
			break;
      }while(c!=0);
    }
	printf("press any key to exit...");
	getchar();
	fclose(fin);
    fclose(fout);
}
