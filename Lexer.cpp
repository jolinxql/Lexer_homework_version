// Lexer.cpp :
//运行时能接收任意被分析的程序，并将词法分析的结果输出到命名为"学号_token.txt"的结果文件中

#include "stdafx.h"

using namespace std;
string sym[]={"IDEN","CONSTTK","INTTK","CHARTK","VARTK","ARRAYTK","OFTK","IFTK","THENTK","ELSETK","DOTK","WHILETK","FORTK","TOTK","BYTK","DOWNTOTK","PROCETK","FUNCTK","READTK","WRITETK","CALLTK","BEGINTK","ENDTK","PLUS","MINU","MULT","DIV","LSS","LEQ","GRE","GEQ","EQL","NEQ","ASSIGN","SEMICN","COMMA","PERIOD","COLON","QMARK","DQMARK","LPARENT","RPARENT","LBRACK","RBRACK","LBRACE","RBRACE","INTCON","CHARCON","STRCON"};//符号类别码
string reservesym[]={"const","integer","char","var","array","of","if","then","else","do","while","for","to","by","downto","procedure","function","read","write","call","begin","end","+","-","*","/","<","<=",">",">=","=","<>",":=",";",",",".",":","\'","\"","(",")","[","]","{","}"};
enum symbol{IDEN,CONSTTK,INTTK,CHARTK,VARTK,ARRAYTK,OFTK,IFTK,THENTK,ELSETK,DOTK,WHILETK,FORTK,TOTK,BYTK,DOWNTOTK,PROCETK,FUNCTK,READTK,WRITETK,CALLTK,BEGINTK,ENDTK,PLUS,MINU,MULT,DIV,LSS,LEQ,GRE,GEQ,EQL,NEQ,ASSIGN,SEMICN,COMMA,PERIOD,COLON,QMARK,DQMARK,LPARENT,RPARENT,LBRACK,RBRACK,LBRACE,RBRACE,INTCON,CHARCON,STRCON};//枚举符号类别码

char l,c,br;//c是下一个读取的字符，l是上一个，br用于debug
int symclass;
string token;
int pt;
char line[LMAX]={0};//一次读取一行


void mygets(FILE *in){ //从文件中单行读取
	char* cin;
	int linept=0,i;

	cin=fgets(line,LMAX,in);
	i=strlen(line);
	if(!cin){line[i]=-1;line[i+1]='\0'; }
	/*cin=fgetc(in);
	for(i=0;i<1000;i++) //字符串清零
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
{return (c>='a'&&c<='z')||(c>'A'&&c<'Z');}//判断字母
bool isDigit()
{return c>='0'&&c<='9';}//判断数字
int reserve(){    //查找是否为保留字
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
  c=line[++pt];//读取一个字符
  //cout<<c;cin>>br;return;
  while(c==' '||c=='\t'||c=='\n') {c=line[++pt];}//跳过空白符再读取一个字符，l始终保存上一个字符
  //cin>>br;return;
  if(c=='\0'||c==-1) return;
  if(isLetter()){                     //判断当前字符是否为字母
    //cout<<c<<" is a letter"<<endl;
    //cin>>br;
    while(isLetter()||isDigit()){   //将字符拼接成字符串
      token=token+c;
      c=line[++pt];
      //cout<<"token is"<<token<<endl;cin>>br;return;
    }
    c=line[--pt];    //后退一个字符
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
      token=token+c;//将字符拼接成整数
      c=line[++pt];
    }
    c=line[--pt];    //后退一个字符
    //注意：在写语法分析时，要做字符串to整形的转换！！！int num=atoi(token.c_str());
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
  else if(c=='\''){		//字符'a'
	c=line[++pt];
	if(isLetter()||isDigit()){
		if(line[pt+1]=='\''){
			//\' 在字符里（即单引号里）使用。在字符串里(即双引号里)不需要，只要用 ' 即可。
			token=c;
			c=line[++pt];
			symclass=CHARCON;
		}
	}
	else{
		lex_error(CHARCON);			//error还需要研究
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
			if(c=='\\'&&(line[pt+1]=='\\'||line[pt+1]=='\'')){		//\' 在字符里（即单引号里）使用。在字符串里(即双引号里)不需要，只要用 ' 即可。
				c=line[++pt];
				token=token+c;
				c=line[++pt];
			}
			else if((c<127&&c>31)&&(c!=34)){		//在字符串所允许的ASCII集中
				token=token+c;
				c=line[++pt];
			}
			else{
				lex_error(STRCON);}	//非法字符
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
  else lex_error(ERRWORD);//其他不合法字符}
  if(symclass>22&&symclass<46) token=reservesym[symclass-1];//不是单词的时候要查符号表来给token赋值
}

class word
{
  public:
    static int ct;//计数器
    string name;//单词名字
  public:
    word(){};
    void set_name(string name)//构造函数，每次传值，计数器加一
    {
      this->name=token;
      ct++;
    }
    void prt_word(FILE *out){   //向文件输出单词行
      fprintf(out,"%d",ct);//打印序号
      string info=" "+sym[symclass]+" "+name+'\n';
      fprintf(out,info.c_str());//“ 类别码 单词名字换行符”
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
	int flag=0;//作为异常的标志直接跳转。=1异常
    FILE *fin,*fout;
    word nextsym;
	fout= fopen(outfilepath,"w");//打开文件用于写，若是"at"就可以实现“如果不存在就创建它并在结尾追加”
	
	//printf("Using default input path...\n>>>Z:\\share\\sae6depart\\6系资料\\编译技术\\大作业\\2011编译技术课设\\2011级编译技术课设\\作业\\词法分析\\lexer\\in.txt\n");
    //strcpy(infilepath,"Z:\\share\\sae6depart\\6系资料\\编译技术\\大作业\\2011编译技术课设\\2011级编译技术课设\\作业\\词法分析\\lexer\\in.txt");

	printf("please enter the source file name...\n");
	gets(infilepath);//得到输入的文件路径
	
	fin=fopen(infilepath,"r");
    while(!fin){//打开失败则重新读入文件路径
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
