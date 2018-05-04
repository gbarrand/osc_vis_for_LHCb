#ifndef lint
static const char Columns_sccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20050813

#define YYEMPTY (-1)
#define Columns_clearin    (Columns_char = YYEMPTY)
#define Columns_errok      (Columns_errflag = 0)
#define YYRECOVERING (Columns_errflag != 0)

//extern int Columns_parse(Lib::Columns& aParser);

static int Columns_growstack(void);
#define YYPREFIX "Columns_"
#line 2 "../source/Columns.y"

/* this :*/
#include <Lib/Columns.h>

#include <Lib/ColumnsLexer.h>

extern int ColumnsLexer_lex(Lib::ColumnsLexer&);
static int Columns_parse(Lib::Columns& aParser);
static void Columns_error(Lib::Columns&,const char*); 

#define NotFound (-1)

#line 16 "../source/Columns.y"
typedef union {
  int fIndex;
  unsigned long fInteger;
  double fNumber;
  Slash::Core::IValue::Type fType;
  Lib::Value* fVariable;
} YYSTYPE;
#line 43 "y.tab.c"
#define STRG 257
#define T_NAME 258
#define NUMBER_INTEGER 259
#define NUMBER_REAL 260
#define T_TRUE 261
#define T_FALSE 262
#define T_AND 263
#define T_OR 264
#define T_SHORT 265
#define T_INT 266
#define T_INT64 267
#define T_FLOAT 268
#define T_DOUBLE 269
#define T_BOOLEAN 270
#define T_BYTE 271
#define T_CHAR 272
#define T_STRING 273
#define T_JAVA_STRING 274
#define T_TUPLE 275
#define UMINUS 276
#define YYERRCODE 256
short Columns_lhs[] = {                                        -1,
    0,    0,    4,    4,    4,    3,    3,    3,    3,    3,
    3,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    2,    2,    2,    2,    2,
};
short Columns_len[] = {                                         2,
    1,    1,    1,    3,    2,    6,    1,    2,    4,    3,
    4,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    1,    2,    1,    1,
};
short Columns_defred[] = {                                      0,
    1,    0,   12,   13,   14,   15,   16,   17,   18,   19,
   20,   21,    0,    0,    0,    0,    2,    0,    0,    0,
    0,    0,   28,   29,   25,   26,   22,   23,    0,   10,
    0,    0,    0,    4,   24,   27,    0,   11,    9,    0,
    6,
};
short Columns_dgoto[] = {                                      14,
   15,   30,   16,   17,
};
short Columns_sindex[] = {                                   -250,
    0,  -54,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -245,    0,  -90,  -35,    0,  -45,  -47,  -33,
  -64, -232,    0,    0,    0,    0,    0,    0, -249,    0,
  -93,  -45, -227,    0,    0,    0, -232,    0,    0,  -81,
    0,
};
short Columns_rindex[] = {                                      0,
    0,    2,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    4,    0,    0,    0,    3,
    0,    5,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
short Columns_gindex[] = {                                      0,
    0,   13,    0,  -10,
};
#define YYTABLESIZE 217
short Columns_table[] = {                                      29,
   21,    7,    8,    3,    5,    1,   18,    2,   22,   35,
   36,   34,   19,   31,    3,    4,    5,    6,    7,    8,
    9,   10,   11,   12,   13,    2,   40,   32,   33,   37,
   39,    0,    3,    4,    5,    6,    7,    8,    9,   10,
   11,   12,   13,   41,   38,    7,    8,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    7,    8,    3,    5,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   20,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   23,   24,   25,   26,   27,   28,
};
short Columns_check[] = {                                      45,
   91,    0,    0,    0,    0,  256,   61,  258,   44,  259,
  260,   22,  258,   61,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  258,   37,   61,   93,  123,
  258,   -1,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,  275,  125,   32,   44,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  125,  125,  125,  125,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  257,  258,  259,  260,  261,  262,
};
#define YYFINAL 14
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 276
#if YYDEBUG
char *Columns_name[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,"'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,0,0,"'='",
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"STRG","T_NAME","NUMBER_INTEGER","NUMBER_REAL","T_TRUE","T_FALSE","T_AND",
"T_OR","T_SHORT","T_INT","T_INT64","T_FLOAT","T_DOUBLE","T_BOOLEAN","T_BYTE",
"T_CHAR","T_STRING","T_JAVA_STRING","T_TUPLE","UMINUS",
};
char *Columns_rule[] = {
"$accept : result",
"result : error",
"result : declarations",
"declarations : declaration",
"declarations : declaration ',' declarations",
"declarations : declaration ','",
"declaration : T_TUPLE T_NAME '=' '{' declarations '}'",
"declaration : T_NAME",
"declaration : type T_NAME",
"declaration : type '[' ']' T_NAME",
"declaration : T_NAME '=' value",
"declaration : type T_NAME '=' value",
"type : T_SHORT",
"type : T_INT",
"type : T_INT64",
"type : T_FLOAT",
"type : T_DOUBLE",
"type : T_BOOLEAN",
"type : T_BYTE",
"type : T_CHAR",
"type : T_STRING",
"type : T_JAVA_STRING",
"value : T_TRUE",
"value : T_FALSE",
"value : '-' NUMBER_INTEGER",
"value : NUMBER_INTEGER",
"value : NUMBER_REAL",
"value : '-' NUMBER_REAL",
"value : STRG",
"value : T_NAME",
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

int      Columns_debug;
int      Columns_nerrs;
int      Columns_errflag;
int      Columns_char;
short   *Columns_ssp;
YYSTYPE *Columns_vsp;
YYSTYPE  Columns_val;
YYSTYPE  Columns_lval;

/* variables for the parser stack */
static short   *Columns_ss;
static short   *Columns_sslim;
static YYSTYPE *Columns_vs;
static int      Columns_stacksize;
#line 288 "../source/Columns.y"

Lib::Columns::Columns(const std::string& aString,std::ostream& a_out,int aVerboseLevel) 
:fVerboseLevel(aVerboseLevel)
,fString(aString)
,f_out(a_out)
,fLexer(a_out,aVerboseLevel)
,fSuccess(false)
,fCurrentType(Slash::Core::IValue::NONE)
{}

Lib::Columns::~Columns() {clear();}

Lib::Columns& Lib::Columns::operator=(const Columns& aColumns) {
  fVerboseLevel = aColumns.fVerboseLevel;
  fLexer = aColumns.fLexer;
  // Reset other things :
  fString = "";
  fSuccess = false;
  fStack.clear();
  fCurrentType = Slash::Core::IValue::NONE;
  return *this;
}

void Lib::Columns::setScript(const std::string& aString) { fString = aString;}

std::string Lib::Columns::script() const { return fString;}

bool Lib::Columns::findVariables() {
  clear();
  if(fString.empty()) return false;
  fLexer.set(fString);
  fSuccess = true;
  fCurrentType = Slash::Core::IValue::NONE;
  Columns_parse(*this);
  if(!fSuccess) {
    clear();
  } else {
    if(fTmpVars.size()) {
      f_out << "Lib::Columns::findVariables :"
            << " stack anomaly " << (unsigned int)fTmpVars.size()
            << std::endl;
    }
  }
  return fSuccess;
}

std::list<Lib::Value> Lib::Columns::result() const {
  std::list<Lib::Value> vars;
  copyColumns(fStack,vars);
  return vars;
}

void Lib::Columns::clear() {
  for(unsigned int index=0;index<fTmpVars.size();index++) 
    delete fTmpVars[index];
  fTmpVars.clear();
  deleteColumns(fStack);
}

bool Lib::Columns::isSuccess() const { return fSuccess;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
std::ostream& Lib::Columns::out() const { return f_out;}

Lib::ColumnsLexer& Lib::Columns::lexer() {return fLexer;}

static void Columns_error(Lib::Columns& aParser,const char* aString) {
  aParser.out() << "Columns : " << aString << std::endl;
  aParser.fSuccess = false;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Lib::Columns::deleteColumns(std::list<Lib::Value>& aVars) {
  std::list<Lib::Value>::iterator it;
  for(it=aVars.begin();it!=aVars.end();++it) {
    if((*it).type()==Slash::Core::IValue::VOID_STAR) {
      std::list<Lib::Value>* vars = 
        (std::list<Lib::Value>*)(*it).get_void_star();
      deleteColumns(*vars);
      delete vars;
      //(*it).set((void*)0);
    }
  }
  aVars.clear();
}

void Lib::Columns::copyColumns(const std::list<Lib::Value>& aFrom,std::list<Lib::Value>& aTo) {
  std::list<Lib::Value>::const_iterator it;
  for(it=aFrom.begin();it!=aFrom.end();++it) {
    if((*it).type()==Slash::Core::IValue::VOID_STAR) {
      std::list<Lib::Value>* vars = new std::list<Lib::Value>();
      Lib::Value v((void*)vars);
      v.setLabel((*it).label());
      aTo.push_back(v);
      std::list<Lib::Value>* p = 
        (std::list<Lib::Value>*)(*it).get_void_star(); 
      copyColumns(*p,*vars);
    } else {
      aTo.push_back(*it);
    }
  }
}
#line 342 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int Columns_growstack(void)
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = Columns_stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = Columns_ssp - Columns_ss;
    newss = (Columns_ss != 0)
          ? (short *)realloc(Columns_ss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    Columns_ss  = newss;
    Columns_ssp = newss + i;
    newvs = (Columns_vs != 0)
          ? (YYSTYPE *)realloc(Columns_vs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    Columns_vs = newvs;
    Columns_vsp = newvs + i;
    Columns_stacksize = newsize;
    Columns_sslim = Columns_ss + newsize - 1;
    return 0;
}

#define YYABORT goto Columns_abort
#define YYREJECT goto Columns_abort
#define YYACCEPT goto Columns_accept
#define YYERROR goto Columns_errlab
int
Columns_parse(Lib::Columns& aParser)
{
    register int Columns_m, Columns_n, Columns_state;
#if YYDEBUG
    register const char *Columns_s;

    if ((Columns_s = getenv("YYDEBUG")) != 0)
    {
        Columns_n = *Columns_s;
        if (Columns_n >= '0' && Columns_n <= '9')
            Columns_debug = Columns_n - '0';
    }
#endif

    Columns_nerrs = 0;
    Columns_errflag = 0;
    Columns_char = YYEMPTY;

    if (Columns_ss == NULL && Columns_growstack()) goto Columns_overflow;
    Columns_ssp = Columns_ss;
    Columns_vsp = Columns_vs;
    *Columns_ssp = Columns_state = 0;

Columns_loop:
    if ((Columns_n = Columns_defred[Columns_state]) != 0) goto Columns_reduce;
    if (Columns_char < 0)
    {
        if ((Columns_char = ColumnsLexer_lex(aParser.lexer())) < 0) Columns_char = 0;
#if YYDEBUG
        if (Columns_debug)
        {
            Columns_s = 0;
            if (Columns_char <= YYMAXTOKEN) Columns_s = Columns_name[Columns_char];
            if (!Columns_s) Columns_s = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, Columns_state, Columns_char, Columns_s);
        }
#endif
    }
    if ((Columns_n = Columns_sindex[Columns_state]) && (Columns_n += Columns_char) >= 0 &&
            Columns_n <= YYTABLESIZE && Columns_check[Columns_n] == Columns_char)
    {
#if YYDEBUG
        if (Columns_debug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, Columns_state, Columns_table[Columns_n]);
#endif
        if (Columns_ssp >= Columns_sslim && Columns_growstack())
        {
            goto Columns_overflow;
        }
        *++Columns_ssp = Columns_state = Columns_table[Columns_n];
        *++Columns_vsp = Columns_lval;
        Columns_char = YYEMPTY;
        if (Columns_errflag > 0)  --Columns_errflag;
        goto Columns_loop;
    }
    if ((Columns_n = Columns_rindex[Columns_state]) && (Columns_n += Columns_char) >= 0 &&
            Columns_n <= YYTABLESIZE && Columns_check[Columns_n] == Columns_char)
    {
        Columns_n = Columns_table[Columns_n];
        goto Columns_reduce;
    }
    if (Columns_errflag) goto Columns_inrecovery;

    Columns_error(aParser,"syntax error");

#ifdef lint
    goto Columns_errlab;
#endif

Columns_errlab:
    ++Columns_nerrs;

Columns_inrecovery:
    if (Columns_errflag < 3)
    {
        Columns_errflag = 3;
        for (;;)
        {
            if ((Columns_n = Columns_sindex[*Columns_ssp]) && (Columns_n += YYERRCODE) >= 0 &&
                    Columns_n <= YYTABLESIZE && Columns_check[Columns_n] == YYERRCODE)
            {
#if YYDEBUG
                if (Columns_debug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *Columns_ssp, Columns_table[Columns_n]);
#endif
                if (Columns_ssp >= Columns_sslim && Columns_growstack())
                {
                    goto Columns_overflow;
                }
                *++Columns_ssp = Columns_state = Columns_table[Columns_n];
                *++Columns_vsp = Columns_lval;
                goto Columns_loop;
            }
            else
            {
#if YYDEBUG
                if (Columns_debug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *Columns_ssp);
#endif
                if (Columns_ssp <= Columns_ss) goto Columns_abort;
                --Columns_ssp;
                --Columns_vsp;
            }
        }
    }
    else
    {
        if (Columns_char == 0) goto Columns_abort;
#if YYDEBUG
        if (Columns_debug)
        {
            Columns_s = 0;
            if (Columns_char <= YYMAXTOKEN) Columns_s = Columns_name[Columns_char];
            if (!Columns_s) Columns_s = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, Columns_state, Columns_char, Columns_s);
        }
#endif
        Columns_char = YYEMPTY;
        goto Columns_loop;
    }

Columns_reduce:
#if YYDEBUG
    if (Columns_debug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, Columns_state, Columns_n, Columns_rule[Columns_n]);
#endif
    Columns_m = Columns_len[Columns_n];
    Columns_val = Columns_vsp[1-Columns_m];
    switch (Columns_n)
    {
case 1:
#line 45 "../source/Columns.y"
{
  /* To reset lex and yacc in case of error.*/
}
break;
case 2:
#line 47 "../source/Columns.y"
{
}
break;
case 3:
#line 51 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : declarations : declaration : " 
        << Columns_vsp[0].fVariable->label() << std::endl;
  }
  aParser.fStack.push_front(*(Columns_vsp[0].fVariable));
  delete Columns_vsp[0].fVariable;
  aParser.fTmpVars.pop_back();
}
break;
case 4:
#line 60 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : dec , decls : " << Columns_vsp[-2].fVariable->label() << std::endl;
  }
  aParser.fStack.push_front(*(Columns_vsp[-2].fVariable));
  delete Columns_vsp[-2].fVariable;
  aParser.fTmpVars.pop_back();
}
break;
case 5:
#line 68 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : dec , decls : " << Columns_vsp[-1].fVariable->label() << std::endl;
  }
  aParser.fStack.push_front(*(Columns_vsp[-1].fVariable));
  delete Columns_vsp[-1].fVariable;
  aParser.fTmpVars.pop_back();
}
break;
case 6:
#line 78 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_TUPLE T_NAME = {decs} : " 
        << aParser.lexer().variable(Columns_vsp[-4].fIndex) << std::endl;
  }
  Lib::Value* v = 
    new Lib::Value((void*)new std::list<Lib::Value>(aParser.fStack));
  v->setLabel(aParser.lexer().variable(Columns_vsp[-4].fIndex));
  aParser.fStack.clear();
  aParser.fCurrentType = Slash::Core::IValue::NONE; /*FIXME*/
  Columns_val.fVariable = v;
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 7:
#line 91 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_NAME : " 
        << aParser.lexer().variable(Columns_vsp[0].fIndex) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable(Columns_vsp[0].fIndex));
  v->setType(aParser.fCurrentType);
  Columns_val.fVariable = v;
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 8:
#line 102 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : type T_NAME : " 
        << aParser.lexer().variable(Columns_vsp[0].fIndex) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable(Columns_vsp[0].fIndex));
  v->setType(Columns_vsp[-1].fType);
  aParser.fCurrentType = Columns_vsp[-1].fType;
  Columns_val.fVariable = v;
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 9:
#line 114 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : type [] T_NAME : " 
        << aParser.lexer().variable(Columns_vsp[0].fIndex) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable(Columns_vsp[0].fIndex));
  Slash::Core::IValue::Type atype;
  if(!Lib::Value::arrayType(Columns_vsp[-3].fType,atype)) {
    aParser.out() << "Lib::Columns::parse : type [] T_NAME :" 
        << " problem to get array type of " << Lib::Value::stype(Columns_vsp[-3].fType)
        << std::endl;
  }
  v->setType(atype);
  aParser.fCurrentType = atype;
  Columns_val.fVariable = v;
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 10:
#line 132 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_NAME = value : " 
        << aParser.lexer().variable(Columns_vsp[-2].fIndex) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable(Columns_vsp[-2].fIndex));
  v->setType(aParser.fCurrentType);
  std::string serror;
  if(!Lib::Value::assign(*v,*(Columns_vsp[0].fVariable),aParser.fError)) 
    aParser.fSuccess = false;
  delete Columns_vsp[0].fVariable;
  Columns_val.fVariable = v;
  aParser.fTmpVars.pop_back();
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 11:
#line 148 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : type T_NAME = value : " 
        << aParser.lexer().variable(Columns_vsp[-2].fIndex) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable(Columns_vsp[-2].fIndex));
  v->setType(Columns_vsp[-3].fType);
  if(!Lib::Value::assign(*v,*(Columns_vsp[0].fVariable),aParser.fError)) 
    aParser.fSuccess = false;
  delete Columns_vsp[0].fVariable;
  aParser.fCurrentType = Columns_vsp[-3].fType;
  Columns_val.fVariable = v;
  aParser.fTmpVars.pop_back();
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 12:
#line 167 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_SHORT" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::SHORT;
}
break;
case 13:
#line 173 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_INT" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::INT;
}
break;
case 14:
#line 179 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_INT64" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::INT64;
}
break;
case 15:
#line 185 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_FLOAT" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::FLOAT;
}
break;
case 16:
#line 191 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_DOUBLE" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::DOUBLE;
}
break;
case 17:
#line 197 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_BOOLEAN" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::BOOL;
}
break;
case 18:
#line 203 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_BYTE" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::UNSIGNED_CHAR;
}
break;
case 19:
#line 209 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_CHAR" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::CHAR;
}
break;
case 20:
#line 215 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_STRING" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::STRING;
}
break;
case 21:
#line 221 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_JAVA_STRING" << std::endl;
  }
  Columns_val.fType = Slash::Core::IValue::STRING;
}
break;
case 22:
#line 229 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_TRUE" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value(true);
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 23:
#line 236 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_FALSE" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value(false);
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 24:
#line 243 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_INTEGER" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value(-(int)Columns_vsp[0].fInteger);
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 25:
#line 250 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_INTEGER" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value((int)Columns_vsp[0].fInteger);
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 26:
#line 257 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_REAL" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value((double)Columns_vsp[0].fNumber);
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 27:
#line 264 "../source/Columns.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_REAL" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value((double)(-1. * Columns_vsp[0].fNumber));
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 28:
#line 271 "../source/Columns.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : STRG" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value(aParser.lexer().variable(Columns_vsp[0].fIndex));
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
case 29:
#line 278 "../source/Columns.y"
{
  /* For char.*/
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : STRG" << std::endl;
  }
  Columns_val.fVariable = new Lib::Value(aParser.lexer().variable(Columns_vsp[0].fIndex));
  aParser.fTmpVars.push_back(Columns_val.fVariable);
}
break;
#line 840 "y.tab.c"
    }
    Columns_ssp -= Columns_m;
    Columns_state = *Columns_ssp;
    Columns_vsp -= Columns_m;
    Columns_m = Columns_lhs[Columns_n];
    if (Columns_state == 0 && Columns_m == 0)
    {
#if YYDEBUG
        if (Columns_debug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        Columns_state = YYFINAL;
        *++Columns_ssp = YYFINAL;
        *++Columns_vsp = Columns_val;
        if (Columns_char < 0)
        {
            if ((Columns_char = ColumnsLexer_lex(aParser.lexer())) < 0) Columns_char = 0;
#if YYDEBUG
            if (Columns_debug)
            {
                Columns_s = 0;
                if (Columns_char <= YYMAXTOKEN) Columns_s = Columns_name[Columns_char];
                if (!Columns_s) Columns_s = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, Columns_char, Columns_s);
            }
#endif
        }
        if (Columns_char == 0) goto Columns_accept;
        goto Columns_loop;
    }
    if ((Columns_n = Columns_gindex[Columns_m]) && (Columns_n += Columns_state) >= 0 &&
            Columns_n <= YYTABLESIZE && Columns_check[Columns_n] == Columns_state)
        Columns_state = Columns_table[Columns_n];
    else
        Columns_state = Columns_dgoto[Columns_m];
#if YYDEBUG
    if (Columns_debug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *Columns_ssp, Columns_state);
#endif
    if (Columns_ssp >= Columns_sslim && Columns_growstack())
    {
        goto Columns_overflow;
    }
    *++Columns_ssp = Columns_state;
    *++Columns_vsp = Columns_val;
    goto Columns_loop;

Columns_overflow:
    Columns_error(aParser,"yacc stack overflow");

Columns_abort:
    return (1);

Columns_accept:
    return (0);
}
