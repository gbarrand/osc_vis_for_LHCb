#ifndef lint
static const char Processor_sccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20050813

#define YYEMPTY (-1)
#define Processor_clearin    (Processor_char = YYEMPTY)
#define Processor_errok      (Processor_errflag = 0)
#define YYRECOVERING (Processor_errflag != 0)

//extern int Processor_parse(Lib::Processor& aParser);

static int Processor_growstack(void);
#define YYPREFIX "Processor_"
#line 2 "../source/Processor.y"

/* this :*/
#include <Lib/Processor.h>

#include <Lib/ProcessorLexer.h>
#include <Lib/ExpFunc.h>

#include <inlib/vmanip>

extern int ProcessorLexer_lex(Lib::ProcessorLexer&);
static int Processor_parse(Lib::Processor& aParser);
static void Processor_error(Lib::Processor&,const char*); 

#define NotFound (-1)

/*#define DEBUG*/

#line 22 "../source/Processor.y"
typedef union {
  unsigned int fIndex;
  unsigned int fInteger;
  double fNumber;
  Lib::Expression* fExpression;
} YYSTYPE;
#line 48 "y.tab.c"
#define STRG 257
#define T_NAME 258
#define NUMBER_INTEGER 259
#define NUMBER_REAL 260
#define T_PI 261
#define T_TRUE 262
#define T_FALSE 263
#define T_AND 264
#define T_OR 265
#define UMINUS 266
#define YYERRCODE 256
short Processor_lhs[] = {                                        -1,
    0,    0,    0,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    2,    2,    2,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,
};
short Processor_len[] = {                                         2,
    1,    1,    1,    2,    4,    3,    4,    3,    3,    1,
    3,    4,    3,    4,    4,    4,    3,    3,    3,    4,
    3,    2,    3,    4,    6,    8,   10,   12,   14,    1,
    1,    1,    1,    1,    1,    1,
};
short Processor_defred[] = {                                      0,
    1,   36,    0,   33,   34,   30,   31,   32,    0,    0,
    0,    0,    0,   10,    0,    0,    0,   22,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    9,   23,    0,    0,
    0,    0,    0,    0,   21,    0,   19,    0,    0,    0,
    0,    0,    0,   24,    0,    0,    0,   20,    0,    0,
    0,    0,    0,   25,    0,    0,   26,    0,    0,   27,
    0,    0,   28,    0,    0,   29,
};
short Processor_dgoto[] = {                                      12,
   13,   14,   20,
};
short Processor_sindex[] = {                                    -30,
    0,    0,  -34,    0,    0,    0,    0,    0,   -7,  -22,
  -22,    0,  -16,    0,  107,   -7,   -7,    0,  -16,  107,
  -21,  100,  -22,  -22,  -31, -111,   -7,   -7,   -7,  -40,
  -45,   37,  -42,   51,   74,   81,    0,    0,  -16,  -16,
  -22,  -22,  -38,  -38,    0,   -7,    0,   -7,   -7,   92,
   -7,   -7,   92,    0,   -7,  -16,  -16,    0,   92,   92,
   92,   92,  129,    0,   -7,  140,    0,   -7,  150,    0,
   -7,  157,    0,   -7,  164,    0,
};
short Processor_rindex[] = {                                      0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   21,    0,   24,    0,    0,    0,    8,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   26,   28,
    0,    0,   14,   27,    0,    0,    0,    0,    0,   12,
    0,    0,   40,    0,    0,   32,   79,    0,   54,   56,
   64,   66,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
short Processor_gindex[] = {                                      0,
   90,    0,  306,
};
#define YYTABLESIZE 380
short Processor_table[] = {                                      17,
   35,   46,   10,   30,    9,   16,   41,    4,   29,   11,
   10,   11,   42,   18,    9,   48,   25,   11,   51,   37,
    2,   25,    9,    3,    0,    6,   17,    8,    0,    0,
    0,    5,   17,   35,    0,    0,    0,    9,   35,   13,
    0,   35,   35,   35,   35,   35,   18,   35,    4,   11,
    0,   18,   11,   16,   18,   12,   18,   18,   18,   17,
   35,   35,   35,   15,   17,   14,    6,   17,    8,   17,
   17,   17,    5,   18,   18,   18,   17,   13,    7,    0,
   13,    9,    0,    0,    0,    0,   17,   17,   17,    0,
   17,   16,    0,   12,   16,    9,   12,   49,    0,   19,
   21,   15,   26,   14,   15,    0,   14,   26,    0,    0,
    0,   52,   39,   40,   54,   30,   28,   55,   27,    7,
   29,   38,   30,   28,   35,   27,    0,   29,    0,    0,
   56,   57,   31,   30,   28,   11,   27,   18,   29,   31,
   38,   30,   28,    0,   27,    0,   29,    0,   30,   28,
   17,   27,    0,   29,    0,    0,    0,    0,    0,   34,
   33,   32,    0,   13,    0,    0,   34,   33,   32,   64,
   30,   28,   65,   27,    0,   29,    0,   16,    0,   12,
   67,   30,   28,   68,   27,    0,   29,   15,    0,   14,
   70,   30,   28,   71,   27,    0,   29,   73,   30,   28,
   74,   27,    0,   29,   76,   30,   28,    0,   27,    0,
   29,    0,    0,    0,    0,    0,    2,    3,    4,    5,
    6,    7,    8,    0,    0,    1,    2,    3,    4,    5,
    6,    7,    8,    0,    2,    3,    4,    5,    6,    7,
    8,    0,   23,   24,    0,    0,    0,   23,   24,    2,
    3,    4,    5,    6,    7,    8,    0,    0,    0,    0,
    0,    0,    0,    0,   35,   35,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   11,   11,   18,   18,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   17,   17,    0,    2,    3,    4,    5,    6,    7,    8,
    0,    0,    0,   13,   13,   15,    0,    2,    3,    4,
    5,    6,    7,    8,   18,    0,   22,   16,   16,   12,
   12,   35,   36,    0,    0,    0,    0,   15,   15,   14,
   14,    0,   43,   44,   45,   47,    0,   50,    0,   53,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   58,    0,   59,   60,    0,   61,   62,    0,    0,
   63,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   66,    0,    0,   69,    0,    0,   72,    0,    0,   75,
};
short Processor_check[] = {                                      40,
    0,   42,   33,   42,   45,   40,   38,    0,   47,   40,
   33,    0,  124,    0,   45,   61,   38,   40,   61,   41,
    0,   38,   45,    0,   -1,    0,    0,    0,   -1,   -1,
   -1,    0,   40,   33,   -1,   -1,   -1,   45,   38,    0,
   -1,   41,   42,   43,   44,   45,   33,   47,   41,   38,
   -1,   38,   41,    0,   41,    0,   43,   44,   45,   33,
   60,   61,   62,    0,   38,    0,   41,   41,   41,   43,
   44,   45,   41,   60,   61,   62,   40,   38,    0,   -1,
   41,   45,   -1,   -1,   -1,   -1,   60,   61,   62,   -1,
   40,   38,   -1,   38,   41,   45,   41,   61,   -1,   10,
   11,   38,  124,   38,   41,   -1,   41,  124,   -1,   -1,
   -1,   61,   23,   24,   41,   42,   43,   44,   45,   41,
   47,   41,   42,   43,  124,   45,   -1,   47,   -1,   -1,
   41,   42,   33,   42,   43,  124,   45,  124,   47,   33,
   41,   42,   43,   -1,   45,   -1,   47,   -1,   42,   43,
  124,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   60,
   61,   62,   -1,  124,   -1,   -1,   60,   61,   62,   41,
   42,   43,   44,   45,   -1,   47,   -1,  124,   -1,  124,
   41,   42,   43,   44,   45,   -1,   47,  124,   -1,  124,
   41,   42,   43,   44,   45,   -1,   47,   41,   42,   43,
   44,   45,   -1,   47,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,  257,  258,  259,  260,
  261,  262,  263,   -1,   -1,  256,  257,  258,  259,  260,
  261,  262,  263,   -1,  257,  258,  259,  260,  261,  262,
  263,   -1,  264,  265,   -1,   -1,   -1,  264,  265,  257,
  258,  259,  260,  261,  262,  263,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  264,  265,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  264,  265,   -1,  257,  258,  259,  260,  261,  262,  263,
   -1,   -1,   -1,  264,  265,    0,   -1,  257,  258,  259,
  260,  261,  262,  263,    9,   -1,   11,  264,  265,  264,
  265,   16,   17,   -1,   -1,   -1,   -1,  264,  265,  264,
  265,   -1,   27,   28,   29,   30,   -1,   32,   -1,   34,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   46,   -1,   48,   49,   -1,   51,   52,   -1,   -1,
   55,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   65,   -1,   -1,   68,   -1,   -1,   71,   -1,   -1,   74,
};
#define YYFINAL 12
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 266
#if YYDEBUG
char *Processor_name[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,0,"'&'",0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,
0,0,0,0,0,"'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'|'",0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"STRG","T_NAME","NUMBER_INTEGER","NUMBER_REAL","T_PI",
"T_TRUE","T_FALSE","T_AND","T_OR","UMINUS",
};
char *Processor_rule[] = {
"$accept : result",
"result : error",
"result : comparisons",
"result : expression",
"comparisons : '!' comparisons",
"comparisons : comparisons '&' '&' comparisons",
"comparisons : comparisons T_AND comparisons",
"comparisons : comparisons '|' '|' comparisons",
"comparisons : comparisons T_OR comparisons",
"comparisons : '(' comparisons ')'",
"comparisons : comparison",
"comparison : expression '>' expression",
"comparison : expression '>' '=' expression",
"comparison : expression '<' expression",
"comparison : expression '<' '=' expression",
"comparison : expression '=' '=' expression",
"comparison : expression '!' '=' expression",
"expression : expression '+' expression",
"expression : expression '-' expression",
"expression : expression '*' expression",
"expression : expression '*' '*' expression",
"expression : expression '/' expression",
"expression : '-' expression",
"expression : '(' expression ')'",
"expression : T_NAME '(' expression ')'",
"expression : T_NAME '(' expression ',' expression ')'",
"expression : T_NAME '(' expression ',' expression ',' expression ')'",
"expression : T_NAME '(' expression ',' expression ',' expression ',' expression ')'",
"expression : T_NAME '(' expression ',' expression ',' expression ',' expression ',' expression ')'",
"expression : T_NAME '(' expression ',' expression ',' expression ',' expression ',' expression ',' expression ')'",
"expression : T_PI",
"expression : T_TRUE",
"expression : T_FALSE",
"expression : NUMBER_INTEGER",
"expression : NUMBER_REAL",
"expression : T_NAME",
"expression : STRG",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

int      Processor_debug;
int      Processor_nerrs;
int      Processor_errflag;
int      Processor_char;
short   *Processor_ssp;
YYSTYPE *Processor_vsp;
YYSTYPE  Processor_val;
YYSTYPE  Processor_lval;

/* variables for the parser stack */
static short   *Processor_ss;
static short   *Processor_sslim;
static YYSTYPE *Processor_vs;
static int      Processor_stacksize;
#line 508 "../source/Processor.y"

Lib::Processor::Processor(
 const std::string& aString
,std::ostream& a_out
) 
:fString(aString)
,f_out(a_out)
,fFindNames(false)
,fFindFunctions(false)
,fLexer(a_out)
,fResult(0)
,fSuccess(false)
,fFuncs(0)
{
  fFuncs = new ExpFuncs();
}

Lib::Processor::~Processor() { 
  clear();
  delete fFuncs;
}

Lib::Processor::Processor(const Processor& aFrom)
:fString("")
,f_out(aFrom.f_out)
,fFindNames(false)
,fFindFunctions(false)
,fLexer(aFrom.fLexer)
,fResult(0)
,fSuccess(false)
,fFuncs(0)
{
  fFuncs = new ExpFuncs(*(aFrom.fFuncs));
}

Lib::Processor& Lib::Processor::operator=(const Processor& aFrom) {
  clear();
  fLexer = aFrom.fLexer;
  delete fFuncs;
  fFuncs = new ExpFuncs(*(aFrom.fFuncs));
  // Reset other things :
  fString = "";
  fFindNames = false;
  fFindFunctions = false;
  fResult = 0;
  return *this;
}

void Lib::Processor::setScript(const std::string& aString) {
  clear();
  fString = aString;
  fFindNames = false;
  fFindFunctions = false;
  fResult = 0;
}

void Lib::Processor::clear() {
 {std::vector<Expression*>::iterator it;
  for(it=fTmpExps.begin();it!=fTmpExps.end();++it) delete (*it);
  fTmpExps.clear();}
  fNames.clear();
  delete fResult;
  fResult = 0;
}

bool Lib::Processor::findVariables(std::vector<std::string>& aNames) {
  aNames.clear();
  clear();
  if(!fString.size()) return true;
  fLexer.set(fString);
  fFindNames = true;
  fFindFunctions = false;
  fSuccess = true;
  Processor_parse(*this);
  if(!fSuccess) {
    clear();
    return false;
  }
  if(fTmpExps.size()) {
    f_out << "Lib::Processor::findVariables :"
        << " stack anomaly " << (unsigned int)fTmpExps.size()
        << std::endl;
  }
  inlib::unique<std::string>(fNames);
  aNames = fNames;
  return true;
}

bool Lib::Processor::findFunctions(std::vector<std::string>& aNames) {
  aNames.clear();
  clear();
  if(!fString.size()) return true;
  fLexer.set(fString);
  fFindNames = false;
  fFindFunctions = true;
  fSuccess = true;
  Processor_parse(*this);
  if(!fSuccess) {
    clear();
    return false;
  }
  if(fTmpExps.size()) {
    f_out << "Lib::Processor::findVariables :"
        << " stack anomaly " << (unsigned int)fTmpExps.size()
        << std::endl;
  }
  inlib::unique<std::string>(fNames);
  aNames = fNames;
  return true;
}

bool Lib::Processor::compile() {
  std::vector<std::string> vars;
  findVariables(vars);
  return compile(vars);
}

bool Lib::Processor::compile(const std::vector<std::string>& aNames) {
  clear();
  if(!fString.size()) return true;
  fNames = aNames;
  fLexer.set(fString);
  fFindNames = false;
  fFindFunctions = false;
  fSuccess = true;
  Processor_parse(*this);
  if(!fSuccess) {
    clear();
    return false;
  }
  if(fTmpExps.size()) {
    f_out << "Lib::Processor::compile :"
        << " stack anomaly " << (unsigned int)fTmpExps.size()
        << std::endl;
  }
  return true;
}

bool Lib::Processor::execute(const std::vector<Lib::Value>& aInput,Lib::Value& aResult,std::string& aError) {
  //printf("debug : Lib::Processor::execute : \"%s\" %lu\n",
  //  fString.c_str(),fResult);
  if(!fString.size()) {
    aResult.setNone();
    return true;
  }
  if(!fResult) {
    aResult.setNone();
    aError = "Lib::Processor::execute : not ready : fReturn = 0.";
    return false;
  }
  if(!fResult->execute(aInput,aError)) {
    aResult.setNone();
    return false;
  }
  if(!fSuccess) {
    aResult.setNone();
    aError = "Lib::Processor::execute : failed.";
    return false;
  }
  aResult = fResult->ret();
  return true;
}

bool Lib::Processor::cut(const std::vector<Lib::Value>& aInput,bool& aResult,std::string& aError) {
  Lib::Value ret; //WARNING : this allocation can take time.
  if(!execute(aInput,ret,aError)) {
    aResult = true;
    return false;
  }
  if(ret.type()==Slash::Core::IValue::BOOL) {
    aResult = ret.get_bool();
    return true;
  }
  if(ret.type()==Slash::Core::IValue::NONE) { 
    //This may happen if fString is empty.
    aResult = true;
    return true;
  }
  aResult = true;
  aError = "Lib::Processor::cut : failed.";
  return false;
}

void Lib::Processor::addName(const std::string& aString) { fNames.push_back(aString);}

std::ostream& Lib::Processor::out() const { return f_out;}

bool Lib::Processor::findNames() const{ return fFindNames;}

bool Lib::Processor::findFunctions() const{ return fFindFunctions;}

Lib::ProcessorLexer& Lib::Processor::lexer() {return fLexer;}

bool Lib::Processor::findVariable(const std::string& aString,unsigned int& aIndex){
  if(!inlib::item_index<std::string>(fNames,aString,aIndex)) {
    f_out << "Lib::Processor::findVariable :" 
        << " " << aString << " not a variable." 
        << std::endl;
    return false;
  }
  return true;
}

std::vector<Lib::Expression*>& Lib::Processor::stack(){ return fTmpExps;}

void Lib::Processor::setResult(Lib::Expression* aValue){ fResult = aValue;}

void Lib::Processor::setStatus(bool aValue){fSuccess = aValue;}

Lib::IExpFunc* Lib::Processor::findFunction(const std::string& aString,bool a_warn) const {

  if(!fFuncs) return 0;
  Lib::IExpFunc* func = fFuncs->find(aString);
  if(!func && a_warn) {
    f_out << "Lib::Processor::findFunction :" 
        << " " << aString << " not a function."
        << std::endl;
    return 0;
  }
  return func;
}

void Lib::Processor::addFunction(Lib::IExpFunc* aFunc) {
  if(!fFuncs) return;
  return fFuncs->add(aFunc);
}

static void Processor_error(Lib::Processor& aParser,const char* aString) {
  aParser.out() << "Processor : " << aString << std::endl;
  aParser.setStatus(false);
}
#line 511 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int Processor_growstack(void)
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = Processor_stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = Processor_ssp - Processor_ss;
    newss = (Processor_ss != 0)
          ? (short *)realloc(Processor_ss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    Processor_ss  = newss;
    Processor_ssp = newss + i;
    newvs = (Processor_vs != 0)
          ? (YYSTYPE *)realloc(Processor_vs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    Processor_vs = newvs;
    Processor_vsp = newvs + i;
    Processor_stacksize = newsize;
    Processor_sslim = Processor_ss + newsize - 1;
    return 0;
}

#define YYABORT goto Processor_abort
#define YYREJECT goto Processor_abort
#define YYACCEPT goto Processor_accept
#define YYERROR goto Processor_errlab
int
Processor_parse(Lib::Processor& aParser)
{
    register int Processor_m, Processor_n, Processor_state;
#if YYDEBUG
    register const char *Processor_s;

    if ((Processor_s = getenv("YYDEBUG")) != 0)
    {
        Processor_n = *Processor_s;
        if (Processor_n >= '0' && Processor_n <= '9')
            Processor_debug = Processor_n - '0';
    }
#endif

    Processor_nerrs = 0;
    Processor_errflag = 0;
    Processor_char = YYEMPTY;

    if (Processor_ss == NULL && Processor_growstack()) goto Processor_overflow;
    Processor_ssp = Processor_ss;
    Processor_vsp = Processor_vs;
    *Processor_ssp = Processor_state = 0;

Processor_loop:
    if ((Processor_n = Processor_defred[Processor_state]) != 0) goto Processor_reduce;
    if (Processor_char < 0)
    {
        if ((Processor_char = ProcessorLexer_lex(aParser.lexer())) < 0) Processor_char = 0;
#if YYDEBUG
        if (Processor_debug)
        {
            Processor_s = 0;
            if (Processor_char <= YYMAXTOKEN) Processor_s = Processor_name[Processor_char];
            if (!Processor_s) Processor_s = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, Processor_state, Processor_char, Processor_s);
        }
#endif
    }
    if ((Processor_n = Processor_sindex[Processor_state]) && (Processor_n += Processor_char) >= 0 &&
            Processor_n <= YYTABLESIZE && Processor_check[Processor_n] == Processor_char)
    {
#if YYDEBUG
        if (Processor_debug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, Processor_state, Processor_table[Processor_n]);
#endif
        if (Processor_ssp >= Processor_sslim && Processor_growstack())
        {
            goto Processor_overflow;
        }
        *++Processor_ssp = Processor_state = Processor_table[Processor_n];
        *++Processor_vsp = Processor_lval;
        Processor_char = YYEMPTY;
        if (Processor_errflag > 0)  --Processor_errflag;
        goto Processor_loop;
    }
    if ((Processor_n = Processor_rindex[Processor_state]) && (Processor_n += Processor_char) >= 0 &&
            Processor_n <= YYTABLESIZE && Processor_check[Processor_n] == Processor_char)
    {
        Processor_n = Processor_table[Processor_n];
        goto Processor_reduce;
    }
    if (Processor_errflag) goto Processor_inrecovery;

    Processor_error(aParser,"syntax error");

#ifdef lint
    goto Processor_errlab;
#endif

Processor_errlab:
    ++Processor_nerrs;

Processor_inrecovery:
    if (Processor_errflag < 3)
    {
        Processor_errflag = 3;
        for (;;)
        {
            if ((Processor_n = Processor_sindex[*Processor_ssp]) && (Processor_n += YYERRCODE) >= 0 &&
                    Processor_n <= YYTABLESIZE && Processor_check[Processor_n] == YYERRCODE)
            {
#if YYDEBUG
                if (Processor_debug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *Processor_ssp, Processor_table[Processor_n]);
#endif
                if (Processor_ssp >= Processor_sslim && Processor_growstack())
                {
                    goto Processor_overflow;
                }
                *++Processor_ssp = Processor_state = Processor_table[Processor_n];
                *++Processor_vsp = Processor_lval;
                goto Processor_loop;
            }
            else
            {
#if YYDEBUG
                if (Processor_debug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *Processor_ssp);
#endif
                if (Processor_ssp <= Processor_ss) goto Processor_abort;
                --Processor_ssp;
                --Processor_vsp;
            }
        }
    }
    else
    {
        if (Processor_char == 0) goto Processor_abort;
#if YYDEBUG
        if (Processor_debug)
        {
            Processor_s = 0;
            if (Processor_char <= YYMAXTOKEN) Processor_s = Processor_name[Processor_char];
            if (!Processor_s) Processor_s = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, Processor_state, Processor_char, Processor_s);
        }
#endif
        Processor_char = YYEMPTY;
        goto Processor_loop;
    }

Processor_reduce:
#if YYDEBUG
    if (Processor_debug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, Processor_state, Processor_n, Processor_rule[Processor_n]);
#endif
    Processor_m = Processor_len[Processor_n];
    Processor_val = Processor_vsp[1-Processor_m];
    switch (Processor_n)
    {
case 1:
#line 46 "../source/Processor.y"
{
  /* To reset lex and yacc in case of error.*/
}
break;
case 2:
#line 48 "../source/Processor.y"
{
  if(aParser.findNames()||aParser.findFunctions()) {
    aParser.setResult(0);
  } else {
    aParser.setResult(Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
  }
}
break;
case 3:
#line 55 "../source/Processor.y"
{
  if(aParser.findNames()||aParser.findFunctions()) {
    aParser.setResult(0);
  } else {
    aParser.setResult(Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
  }
}
break;
case 4:
#line 65 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : ! cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::NOT,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 5:
#line 76 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps && cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::AND,Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 6:
#line 88 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps && cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::AND,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 7:
#line 100 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps || cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::OR,Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 8:
#line 112 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps || cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::OR,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 9:
#line 124 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : (cps)" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::ASSIGN,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 10:
#line 135 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::ASSIGN,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 11:
#line 149 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp > exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::GT,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 12:
#line 161 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp >= exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::GE,Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 13:
#line 173 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp < exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::LT,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 14:
#line 185 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp <= exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::LE,Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 15:
#line 197 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp == exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::EQ,Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 16:
#line 209 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp != exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::NE,Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 17:
#line 224 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp + exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::ADD,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 18:
#line 236 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp - exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::SUB,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 19:
#line 248 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp * exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::MUL,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 20:
#line 260 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp ** exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction("pow"),Processor_vsp[-3].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 21:
#line 273 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp / exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::DIV,Processor_vsp[-2].fExpression,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 22:
#line 285 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : - exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::MINUS,Processor_vsp[0].fExpression);
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 23:
#line 296 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : (exp)" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::ASSIGN,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }

}
break;
case 24:
#line 308 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_COS" << std::endl;
#endif
  if(aParser.findNames()) {
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[-3].fIndex));
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[-3].fIndex);
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 25:
#line 325 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_POW" << std::endl;
#endif
  if(aParser.findNames()) {
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[-5].fIndex));
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[-5].fIndex);    
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       Processor_vsp[-3].fExpression,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 26:
#line 343 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_EXPO" << std::endl;
#endif
  if(aParser.findNames()) {
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[-7].fIndex));
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[-7].fIndex);    
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       Processor_vsp[-5].fExpression,Processor_vsp[-3].fExpression,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 27:
#line 362 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_GAUSS" << std::endl;
#endif
  if(aParser.findNames()) {
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[-9].fIndex));
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[-9].fIndex);    
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       Processor_vsp[-7].fExpression,Processor_vsp[-5].fExpression,Processor_vsp[-3].fExpression,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 28:
#line 382 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_POL3" << std::endl;
#endif
  if(aParser.findNames()) {
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[-11].fIndex));
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[-11].fIndex);
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       Processor_vsp[-9].fExpression,Processor_vsp[-7].fExpression,Processor_vsp[-5].fExpression,Processor_vsp[-3].fExpression,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 29:
#line 403 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_POL4" << std::endl;
#endif
  if(aParser.findNames()) {
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[-13].fIndex));
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[-13].fIndex);    
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       Processor_vsp[-11].fExpression,Processor_vsp[-9].fExpression,Processor_vsp[-7].fExpression,Processor_vsp[-5].fExpression,Processor_vsp[-3].fExpression,Processor_vsp[-1].fExpression);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back(Processor_val.fExpression);
  }

}
break;
case 30:
#line 426 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_PI" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::PI);
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 31:
#line 436 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_TRUE" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::BOOL_TRUE);
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 32:
#line 446 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_FALSE" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::BOOL_FALSE);
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 33:
#line 456 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : NUMBER_INTEGER" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression
      (Lib::Expression::UNSIGNED_INTEGER,(unsigned int)Processor_vsp[0].fInteger);
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 34:
#line 467 "../source/Processor.y"
{ 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : NUMBER_REAL" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::REAL,Processor_vsp[0].fNumber);
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 35:
#line 477 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_NAME" << std::endl;
#endif
  if(aParser.findNames()) {
    aParser.addName(aParser.lexer().variable(Processor_vsp[0].fIndex));
    Processor_val.fExpression = 0;
  } else if(aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    std::string var = aParser.lexer().variable(Processor_vsp[0].fIndex);
    unsigned int index;
    int ivar = NotFound;
    if(aParser.findVariable(var,index)) ivar = int(index);
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::NAME,var,ivar);
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
case 36:
#line 494 "../source/Processor.y"
{
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : STRG" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    Processor_val.fExpression = 0;
  } else {
    Processor_val.fExpression = new Lib::Expression(Lib::Expression::STRING,
                             aParser.lexer().variable(Processor_vsp[0].fIndex));
    aParser.stack().push_back(Processor_val.fExpression);
  }
}
break;
#line 1283 "y.tab.c"
    }
    Processor_ssp -= Processor_m;
    Processor_state = *Processor_ssp;
    Processor_vsp -= Processor_m;
    Processor_m = Processor_lhs[Processor_n];
    if (Processor_state == 0 && Processor_m == 0)
    {
#if YYDEBUG
        if (Processor_debug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        Processor_state = YYFINAL;
        *++Processor_ssp = YYFINAL;
        *++Processor_vsp = Processor_val;
        if (Processor_char < 0)
        {
            if ((Processor_char = ProcessorLexer_lex(aParser.lexer())) < 0) Processor_char = 0;
#if YYDEBUG
            if (Processor_debug)
            {
                Processor_s = 0;
                if (Processor_char <= YYMAXTOKEN) Processor_s = Processor_name[Processor_char];
                if (!Processor_s) Processor_s = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, Processor_char, Processor_s);
            }
#endif
        }
        if (Processor_char == 0) goto Processor_accept;
        goto Processor_loop;
    }
    if ((Processor_n = Processor_gindex[Processor_m]) && (Processor_n += Processor_state) >= 0 &&
            Processor_n <= YYTABLESIZE && Processor_check[Processor_n] == Processor_state)
        Processor_state = Processor_table[Processor_n];
    else
        Processor_state = Processor_dgoto[Processor_m];
#if YYDEBUG
    if (Processor_debug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *Processor_ssp, Processor_state);
#endif
    if (Processor_ssp >= Processor_sslim && Processor_growstack())
    {
        goto Processor_overflow;
    }
    *++Processor_ssp = Processor_state;
    *++Processor_vsp = Processor_val;
    goto Processor_loop;

Processor_overflow:
    Processor_error(aParser,"yacc stack overflow");

Processor_abort:
    return (1);

Processor_accept:
    return (0);
}
