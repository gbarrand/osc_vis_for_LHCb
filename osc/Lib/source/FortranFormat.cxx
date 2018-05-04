#ifndef lint
static const char FortranFormat_sccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20050813

#define YYEMPTY (-1)
#define FortranFormat_clearin    (FortranFormat_char = YYEMPTY)
#define FortranFormat_errok      (FortranFormat_errflag = 0)
#define YYRECOVERING (FortranFormat_errflag != 0)

//extern int FortranFormat_parse(Lib::FortranFormat& aParser);

static int FortranFormat_growstack(void);
#define YYPREFIX "FortranFormat_"
#line 2 "../source/FortranFormat.y"

/* this :*/
#include <Lib/FortranFormat.h>

#include <inlib/sprintf>
#include <inlib/cast>

#include <Lib/FortranFormatLexer.h>
#include <Lib/Debug.h>

extern int FortranFormatLexer_lex(Lib::FortranFormatLexer&);
static int FortranFormat_parse(Lib::FortranFormat& aParser);
static void FortranFormat_error(Lib::FortranFormat&,const char*); 

/*#define LIB_DEBUG*/

#line 20 "../source/FortranFormat.y"
typedef union {
  unsigned long fInteger;
  Lib::DataDescriptor* fDesc;
  unsigned int fType;
} YYSTYPE;
#line 45 "y.tab.c"
#define NUMBER_INTEGER 257
#define NUMBER_EXPONENT 258
#define T_X 259
#define T_I 260
#define T_F 261
#define T_E 262
#define T_G 263
#define T_A 264
#define T_L 265
#define T_SLASH 266
#define UMINUS 267
#define YYERRCODE 256
short FortranFormat_lhs[] = {                                        -1,
    0,    0,    1,    1,    2,    2,    2,    3,    3,    4,
    4,    4,    4,    5,    5,    5,    5,    5,    5,    5,
    5,
};
short FortranFormat_len[] = {                                         2,
    1,    1,    3,    4,    1,    1,    3,    1,    2,    1,
    2,    4,    5,    1,    1,    1,    1,    1,    1,    1,
    1,
};
short FortranFormat_defred[] = {                                      0,
    1,    0,    0,    0,    2,    0,    0,   14,   15,   16,
   17,   18,   19,   20,   21,    6,    0,    5,    8,    0,
    0,    9,    3,    0,    0,    4,    0,    0,    0,   13,
};
short FortranFormat_dgoto[] = {                                       4,
   16,   17,   18,   19,   20,
};
short FortranFormat_sindex[] = {                                    -21,
    0,  -39,  -40,    0,    0,  -40,  -32,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -38,    0,    0, -252,
  -37,    0,    0,  -40,  -28,    0,  -35, -241, -237,    0,
};
short FortranFormat_rindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -31,
    0,    0,    0,    0,  -30,    0,  -24,    0,  -29,    0,
};
short FortranFormat_gindex[] = {                                      0,
   22,   -4,    0,   16,    0,
};
#define YYTABLESIZE 236
short FortranFormat_table[] = {                                       3,
    6,   21,   23,   26,   25,   24,   24,    6,   24,   10,
   11,   12,   10,   11,   12,   29,    7,   28,    3,   27,
   30,    5,   22,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    7,    0,    8,    9,
   10,   11,   12,   13,   14,   15,    8,    9,   10,   11,
   12,   13,   14,   15,    1,    2,
};
short FortranFormat_check[] = {                                      40,
   40,    6,   41,   41,  257,   44,   44,   40,   44,   41,
   41,   41,   44,   44,   44,  257,   41,   46,   40,   24,
  258,    0,    7,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,  259,  260,
  261,  262,  263,  264,  265,  266,  259,  260,  261,  262,
  263,  264,  265,  266,  256,  257,
};
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 267
#if YYDEBUG
char *FortranFormat_name[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"NUMBER_INTEGER","NUMBER_EXPONENT","T_X","T_I","T_F","T_E","T_G","T_A","T_L",
"T_SLASH","UMINUS",
};
char *FortranFormat_rule[] = {
"$accept : result",
"result : error",
"result : data_desc_tree",
"data_desc_tree : '(' data_desc_list ')'",
"data_desc_tree : NUMBER_INTEGER '(' data_desc_list ')'",
"data_desc_list : data_desc_leaf",
"data_desc_list : data_desc_tree",
"data_desc_list : data_desc_list ',' data_desc_list",
"data_desc_leaf : data_desc_one",
"data_desc_leaf : NUMBER_INTEGER data_desc_one",
"data_desc_one : data_type",
"data_desc_one : data_type NUMBER_INTEGER",
"data_desc_one : data_type NUMBER_INTEGER '.' NUMBER_INTEGER",
"data_desc_one : data_type NUMBER_INTEGER '.' NUMBER_INTEGER NUMBER_EXPONENT",
"data_type : T_X",
"data_type : T_I",
"data_type : T_F",
"data_type : T_E",
"data_type : T_G",
"data_type : T_A",
"data_type : T_L",
"data_type : T_SLASH",
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

int      FortranFormat_debug;
int      FortranFormat_nerrs;
int      FortranFormat_errflag;
int      FortranFormat_char;
short   *FortranFormat_ssp;
YYSTYPE *FortranFormat_vsp;
YYSTYPE  FortranFormat_val;
YYSTYPE  FortranFormat_lval;

/* variables for the parser stack */
static short   *FortranFormat_ss;
static short   *FortranFormat_sslim;
static YYSTYPE *FortranFormat_vs;
static int      FortranFormat_stacksize;
#line 241 "../source/FortranFormat.y"

//////////////////////////////////////////////////////////////////////////////
Lib::FortranFormat::FortranFormat(
 const std::string& aString
,std::ostream& a_out
,int aVerboseLevel
) 
:fVerboseLevel(aVerboseLevel)
,fString(aString)
,f_out(a_out)
,fLexer(a_out,aVerboseLevel)
,fSuccess(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Lib::FortranFormat::~FortranFormat(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
}

// private.
Lib::FortranFormat::FortranFormat(const FortranFormat& aFrom) 
:fVerboseLevel(aFrom.fVerboseLevel)
,fString(aFrom.fString)
,f_out(aFrom.f_out)
,fLexer(aFrom.f_out,fVerboseLevel)
,fSuccess(false)
{}

// private.
Lib::FortranFormat& Lib::FortranFormat::operator=(const FortranFormat&) {return *this;}

void Lib::FortranFormat::setScript(const std::string& aString) {fString = aString;}

const std::string& Lib::FortranFormat::script() const {return fString;}

bool Lib::FortranFormat::parse() {
  clear();
  if(fString.empty()) return false;
  fLexer.set(fString);
  fSuccess = true;
  FortranFormat_parse(*this);
  if(!fSuccess) clear();
  return fSuccess;
}

const Lib::DataDescriptor& Lib::FortranFormat::result() const {return fTree;}

void Lib::FortranFormat::clear() {
  fTree.clear();

  // In case of error :
 {std::vector<DataDescriptor*>::iterator it;
  for(it=fTmpDescs.begin();it!=fTmpDescs.end();++it) delete (*it);
  fTmpDescs.clear();}
}

bool Lib::FortranFormat::isSuccess() const {return fSuccess;}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

std::ostream& Lib::FortranFormat::out() const {return f_out;}

Lib::FortranFormatLexer& Lib::FortranFormat::lexer() {return fLexer;}

static void FortranFormat_error(Lib::FortranFormat& aParser,const char* aString) {
  aParser.out() << "FortranFormat : " << aString << std::endl;
  aParser.fSuccess = false;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor::DataDescriptor(
)
:fParent(0)
,fIndex(0)
,fMultiplicity(1)
,fType(0) //Unknown by default
,fWidth(0)
,fPrecision(0)
,fExponent(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("Lib::DataDescriptor");
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor::~DataDescriptor(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  Lib::Debug::decrement("Lib::DataDescriptor");
}

void Lib::DataDescriptor::clear() {
  std::vector<DataDescriptor*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) delete *it;
  fChildren.clear();
}

void Lib::DataDescriptor::reset(bool aDoThis){
  // Descent the tree to reset all the fIndex to 0.
  if(aDoThis) fIndex = 0;
  std::vector<DataDescriptor*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) (*it)->reset();
}

Lib::DataDescriptorIterator* Lib::DataDescriptor::createIterator() const {
  INLIB_SELF(DataDescriptor);
  return new Lib::DataDescriptorIterator(self);
}

Lib::DataDescriptor* Lib::DataDescriptor::firstLeaf() {
  std::vector<DataDescriptor*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    Lib::DataDescriptor* leaf = (*it)->firstLeaf();
    if(leaf) return leaf;
  }
  if(!fChildren.size()) return 0;
  return fChildren[0];
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor* Lib::DataDescriptor::nextChild(
 Lib::DataDescriptor* aChild
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = fChildren.size(); 
  if(!aChild && number) return fChildren[0];
  for(unsigned int index=0;index<number;index++) {
    if(aChild==fChildren[index]) {
      if((index+1)<number) {
        return fChildren[index+1];
      } else { 
        return 0;
      }
    }
  }
  return 0; //Should never pass here.
}
//////////////////////////////////////////////////////////////////////////////
void Lib::DataDescriptor::dump(
 std::ostream& a_out
,int aIndent
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string sindent(aIndent,' ');

  if(!fChildren.size()) { // Leaf :
    a_out << sindent;
    a_out << fMultiplicity;
    if(fType==DataDescriptor_X) a_out << "X";
    else if(fType==DataDescriptor_I) a_out << "I";
    else if(fType==DataDescriptor_F) a_out << "F";
    else if(fType==DataDescriptor_E) a_out << "E";
    else if(fType==DataDescriptor_G) a_out << "G";
    else if(fType==DataDescriptor_A) a_out << "A";
    else if(fType==DataDescriptor_SLASH) a_out << "/";
    else a_out << "?";
    a_out << fWidth;
    a_out << ".";
    a_out << fPrecision;
    a_out << "E";
    a_out << fExponent;
    a_out << std::endl;    
  } else {

    a_out << sindent;
    a_out << fMultiplicity;
    a_out << std::endl;    

   {std::vector<DataDescriptor*>::const_iterator it;
    for(it=fChildren.begin();it!=fChildren.end();++it) {
      (*it)->dump(a_out,aIndent+2);
    }}
  }

}

std::string Lib::DataDescriptor::cformat() const {
  INLIB_SELF(DataDescriptor);

       if(fType==DataDescriptor_X) {
    return "";
  } else if(fType==DataDescriptor_I) {
    std::string cformat;
    inlib::sprintf(cformat,64,"%%%d.%dd",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_F) {
    // # : to force having a decimal point, even if precision is 0.
    std::string cformat;
    inlib::sprintf(cformat,64,"%%#%d.%df",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_E) {
    std::string cformat;
    inlib::sprintf(cformat,64,"%%%d.%dE",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_G) {
    std::string cformat;
    inlib::sprintf(cformat,64,"%%%d.%dG",fWidth,fPrecision);
    return cformat;
  } else if(fType==DataDescriptor_A) {
    return "%s";
  } else if(fType==DataDescriptor_SLASH) {
    return "";
  } else {
    return "";  
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptorIterator::DataDescriptorIterator(
 Lib::DataDescriptor& aTree
)
:fTree(aTree)
,fDesc(0)
,fFinished(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("Lib::DataDescriptorIterator");
  fTree.reset();
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptorIterator::~DataDescriptorIterator(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::decrement("Lib::DataDescriptorIterator");
}
//////////////////////////////////////////////////////////////////////////////
void Lib::DataDescriptorIterator::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTree.reset();
  fDesc = 0;
  fFinished = false;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::DataDescriptorIterator::finished(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFinished;
}
//////////////////////////////////////////////////////////////////////////////
Lib::DataDescriptor* Lib::DataDescriptorIterator::next(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFinished) return 0;
  if(!fDesc) { //Begin of iteration.
    fDesc = &fTree;
    fDesc->fIndex = 0;
  } else { // Repeat again ?
    if((fDesc->fIndex+1)<(int)fDesc->fMultiplicity) {
      fDesc->fIndex++;
      fDesc->reset(false);
      return fDesc; // The same.
    }
  }
  // New desc :
  while(true) {
    Lib::DataDescriptor* leaf = fDesc->firstLeaf();
    if(leaf) {
      fDesc = leaf;
      fDesc->fIndex = 0;
   /*{Lib::DataDescriptor* parent = fDesc->fParent;
      while(parent) {
        parent->fIndex = 0;
        parent = parent->fParent;          
      }}*/
      return leaf;
    }
    // Back to parent and after current fDesc :
    Lib::DataDescriptor* parent = fDesc->fParent;
    while(1) {
      if(!parent) { //Tree finished.      
        fDesc = 0;
        fFinished = true;
        return 0;
      }
      fDesc = parent->nextChild(fDesc);
      if(fDesc) break;
      // parent children exhausted.
      // Repeat again ?
      if((parent->fIndex+1)<(int)parent->fMultiplicity) {
        parent->fIndex++;
        parent->reset(false);
        fDesc = parent;
        break;
      } else {
        fDesc = parent;
        parent = parent->fParent;          
      }
    }
  }
}
#line 535 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int FortranFormat_growstack(void)
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = FortranFormat_stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = FortranFormat_ssp - FortranFormat_ss;
    newss = (FortranFormat_ss != 0)
          ? (short *)realloc(FortranFormat_ss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    FortranFormat_ss  = newss;
    FortranFormat_ssp = newss + i;
    newvs = (FortranFormat_vs != 0)
          ? (YYSTYPE *)realloc(FortranFormat_vs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    FortranFormat_vs = newvs;
    FortranFormat_vsp = newvs + i;
    FortranFormat_stacksize = newsize;
    FortranFormat_sslim = FortranFormat_ss + newsize - 1;
    return 0;
}

#define YYABORT goto FortranFormat_abort
#define YYREJECT goto FortranFormat_abort
#define YYACCEPT goto FortranFormat_accept
#define YYERROR goto FortranFormat_errlab
int
FortranFormat_parse(Lib::FortranFormat& aParser)
{
    register int FortranFormat_m, FortranFormat_n, FortranFormat_state;
#if YYDEBUG
    register const char *FortranFormat_s;

    if ((FortranFormat_s = getenv("YYDEBUG")) != 0)
    {
        FortranFormat_n = *FortranFormat_s;
        if (FortranFormat_n >= '0' && FortranFormat_n <= '9')
            FortranFormat_debug = FortranFormat_n - '0';
    }
#endif

    FortranFormat_nerrs = 0;
    FortranFormat_errflag = 0;
    FortranFormat_char = YYEMPTY;

    if (FortranFormat_ss == NULL && FortranFormat_growstack()) goto FortranFormat_overflow;
    FortranFormat_ssp = FortranFormat_ss;
    FortranFormat_vsp = FortranFormat_vs;
    *FortranFormat_ssp = FortranFormat_state = 0;

FortranFormat_loop:
    if ((FortranFormat_n = FortranFormat_defred[FortranFormat_state]) != 0) goto FortranFormat_reduce;
    if (FortranFormat_char < 0)
    {
        if ((FortranFormat_char = FortranFormatLexer_lex(aParser.lexer())) < 0) FortranFormat_char = 0;
#if YYDEBUG
        if (FortranFormat_debug)
        {
            FortranFormat_s = 0;
            if (FortranFormat_char <= YYMAXTOKEN) FortranFormat_s = FortranFormat_name[FortranFormat_char];
            if (!FortranFormat_s) FortranFormat_s = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, FortranFormat_state, FortranFormat_char, FortranFormat_s);
        }
#endif
    }
    if ((FortranFormat_n = FortranFormat_sindex[FortranFormat_state]) && (FortranFormat_n += FortranFormat_char) >= 0 &&
            FortranFormat_n <= YYTABLESIZE && FortranFormat_check[FortranFormat_n] == FortranFormat_char)
    {
#if YYDEBUG
        if (FortranFormat_debug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, FortranFormat_state, FortranFormat_table[FortranFormat_n]);
#endif
        if (FortranFormat_ssp >= FortranFormat_sslim && FortranFormat_growstack())
        {
            goto FortranFormat_overflow;
        }
        *++FortranFormat_ssp = FortranFormat_state = FortranFormat_table[FortranFormat_n];
        *++FortranFormat_vsp = FortranFormat_lval;
        FortranFormat_char = YYEMPTY;
        if (FortranFormat_errflag > 0)  --FortranFormat_errflag;
        goto FortranFormat_loop;
    }
    if ((FortranFormat_n = FortranFormat_rindex[FortranFormat_state]) && (FortranFormat_n += FortranFormat_char) >= 0 &&
            FortranFormat_n <= YYTABLESIZE && FortranFormat_check[FortranFormat_n] == FortranFormat_char)
    {
        FortranFormat_n = FortranFormat_table[FortranFormat_n];
        goto FortranFormat_reduce;
    }
    if (FortranFormat_errflag) goto FortranFormat_inrecovery;

    FortranFormat_error(aParser,"syntax error");

#ifdef lint
    goto FortranFormat_errlab;
#endif

FortranFormat_errlab:
    ++FortranFormat_nerrs;

FortranFormat_inrecovery:
    if (FortranFormat_errflag < 3)
    {
        FortranFormat_errflag = 3;
        for (;;)
        {
            if ((FortranFormat_n = FortranFormat_sindex[*FortranFormat_ssp]) && (FortranFormat_n += YYERRCODE) >= 0 &&
                    FortranFormat_n <= YYTABLESIZE && FortranFormat_check[FortranFormat_n] == YYERRCODE)
            {
#if YYDEBUG
                if (FortranFormat_debug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *FortranFormat_ssp, FortranFormat_table[FortranFormat_n]);
#endif
                if (FortranFormat_ssp >= FortranFormat_sslim && FortranFormat_growstack())
                {
                    goto FortranFormat_overflow;
                }
                *++FortranFormat_ssp = FortranFormat_state = FortranFormat_table[FortranFormat_n];
                *++FortranFormat_vsp = FortranFormat_lval;
                goto FortranFormat_loop;
            }
            else
            {
#if YYDEBUG
                if (FortranFormat_debug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *FortranFormat_ssp);
#endif
                if (FortranFormat_ssp <= FortranFormat_ss) goto FortranFormat_abort;
                --FortranFormat_ssp;
                --FortranFormat_vsp;
            }
        }
    }
    else
    {
        if (FortranFormat_char == 0) goto FortranFormat_abort;
#if YYDEBUG
        if (FortranFormat_debug)
        {
            FortranFormat_s = 0;
            if (FortranFormat_char <= YYMAXTOKEN) FortranFormat_s = FortranFormat_name[FortranFormat_char];
            if (!FortranFormat_s) FortranFormat_s = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, FortranFormat_state, FortranFormat_char, FortranFormat_s);
        }
#endif
        FortranFormat_char = YYEMPTY;
        goto FortranFormat_loop;
    }

FortranFormat_reduce:
#if YYDEBUG
    if (FortranFormat_debug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, FortranFormat_state, FortranFormat_n, FortranFormat_rule[FortranFormat_n]);
#endif
    FortranFormat_m = FortranFormat_len[FortranFormat_n];
    FortranFormat_val = FortranFormat_vsp[1-FortranFormat_m];
    switch (FortranFormat_n)
    {
case 1:
#line 43 "../source/FortranFormat.y"
{
  /* To reset lex and yacc in case of error.*/
}
break;
case 2:
#line 45 "../source/FortranFormat.y"
{
  aParser.fTree.fChildren.push_back(FortranFormat_vsp[0].fDesc);
  FortranFormat_vsp[0].fDesc->fParent = &(aParser.fTree);
  aParser.fTmpDescs.pop_back();
}
break;
case 3:
#line 52 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_tree : (data_desc_list)" 
        << std::endl;
  }
  FortranFormat_vsp[-1].fDesc->fMultiplicity = 1;
  FortranFormat_val.fDesc = FortranFormat_vsp[-1].fDesc;
}
break;
case 4:
#line 60 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_tree : integer(data_desc_list)" 
        << std::endl;
  }
  FortranFormat_vsp[-1].fDesc->fMultiplicity = FortranFormat_vsp[-3].fInteger;
  FortranFormat_val.fDesc = FortranFormat_vsp[-1].fDesc;
}
break;
case 5:
#line 71 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_list : data_desc_leaf" 
        << std::endl;
  }
  Lib::DataDescriptor* list = new Lib::DataDescriptor();
  list->fChildren.push_back(FortranFormat_vsp[0].fDesc);
  FortranFormat_vsp[0].fDesc->fParent = list;
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.push_back(list);/*To cleanup in case of error.*/
  FortranFormat_val.fDesc = list;
}
break;
case 6:
#line 83 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_list : data_desc_tree" 
        << std::endl;
  }
  Lib::DataDescriptor* list = new Lib::DataDescriptor();
  list->fChildren.push_back(FortranFormat_vsp[0].fDesc);
  FortranFormat_vsp[0].fDesc->fParent = list;
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.push_back(list);/*To cleanup in case of error.*/
  FortranFormat_val.fDesc = list;
}
break;
case 7:
#line 95 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_list : data_desc_tree,data_desc_tree" 
        << std::endl;
  }
  Lib::DataDescriptor* list = new Lib::DataDescriptor();
  list->fChildren.push_back(FortranFormat_vsp[-2].fDesc);
  FortranFormat_vsp[-2].fDesc->fParent = list;
  list->fChildren.push_back(FortranFormat_vsp[0].fDesc);
  FortranFormat_vsp[0].fDesc->fParent = list;
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.pop_back();
  aParser.fTmpDescs.push_back(list);/*To cleanup in case of error.*/
  FortranFormat_val.fDesc = list;
}
break;
case 8:
#line 113 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_leaf : data_desc_one" 
        << std::endl;
  }
  FortranFormat_vsp[0].fDesc->fMultiplicity = 1;
  FortranFormat_val.fDesc = FortranFormat_vsp[0].fDesc;
}
break;
case 9:
#line 121 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_leaf : integer data_desc_one" 
        << std::endl;
  }
  FortranFormat_vsp[0].fDesc->fMultiplicity = FortranFormat_vsp[-1].fInteger;
  FortranFormat_val.fDesc = FortranFormat_vsp[0].fDesc;
}
break;
case 10:
#line 132 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);/*To cleanup in case of error.*/
  leaf->fType = FortranFormat_vsp[0].fType;
  FortranFormat_val.fDesc = leaf;
}
break;
case 11:
#line 142 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type integer" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);/*To cleanup in case of error.*/
  leaf->fType = FortranFormat_vsp[-1].fType;
  leaf->fWidth = FortranFormat_vsp[0].fInteger;
  FortranFormat_val.fDesc = leaf;
}
break;
case 12:
#line 153 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type integer.integer" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);/*To cleanup in case of error.*/
  leaf->fType = FortranFormat_vsp[-3].fType;
  leaf->fWidth = FortranFormat_vsp[-2].fInteger;
  leaf->fPrecision = FortranFormat_vsp[0].fInteger;
  FortranFormat_val.fDesc = leaf;
}
break;
case 13:
#line 165 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_desc_one : data_type integer.integerEinteger" 
        << std::endl;
  }
  Lib::DataDescriptor* leaf = new Lib::DataDescriptor();
  aParser.fTmpDescs.push_back(leaf);/*To cleanup in case of error.*/
  leaf->fType = FortranFormat_vsp[-4].fType;
  leaf->fWidth = FortranFormat_vsp[-3].fInteger;
  leaf->fPrecision = FortranFormat_vsp[-1].fInteger;
  leaf->fExponent = FortranFormat_vsp[0].fInteger;
  FortranFormat_val.fDesc = leaf;
}
break;
case 14:
#line 181 "../source/FortranFormat.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_X" 
        << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_X;
}
break;
case 15:
#line 188 "../source/FortranFormat.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
          << " data_type : T_I" 
          << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_I;
}
break;
case 16:
#line 195 "../source/FortranFormat.y"
{ 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_F" 
        << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_F;
}
break;
case 17:
#line 202 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_E" 
        << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_E;
}
break;
case 18:
#line 209 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_G" 
        << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_G;
}
break;
case 19:
#line 216 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
          << " data_type : T_A" 
          << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_A;
}
break;
case 20:
#line 223 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
          << " data_type : T_L" 
          << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_L;
}
break;
case 21:
#line 230 "../source/FortranFormat.y"
{
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::FortranFormat::parse :"
        << " data_type : T_SLASH" 
        << std::endl;
  }
  FortranFormat_val.fType = DataDescriptor_SLASH;
}
break;
#line 976 "y.tab.c"
    }
    FortranFormat_ssp -= FortranFormat_m;
    FortranFormat_state = *FortranFormat_ssp;
    FortranFormat_vsp -= FortranFormat_m;
    FortranFormat_m = FortranFormat_lhs[FortranFormat_n];
    if (FortranFormat_state == 0 && FortranFormat_m == 0)
    {
#if YYDEBUG
        if (FortranFormat_debug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        FortranFormat_state = YYFINAL;
        *++FortranFormat_ssp = YYFINAL;
        *++FortranFormat_vsp = FortranFormat_val;
        if (FortranFormat_char < 0)
        {
            if ((FortranFormat_char = FortranFormatLexer_lex(aParser.lexer())) < 0) FortranFormat_char = 0;
#if YYDEBUG
            if (FortranFormat_debug)
            {
                FortranFormat_s = 0;
                if (FortranFormat_char <= YYMAXTOKEN) FortranFormat_s = FortranFormat_name[FortranFormat_char];
                if (!FortranFormat_s) FortranFormat_s = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, FortranFormat_char, FortranFormat_s);
            }
#endif
        }
        if (FortranFormat_char == 0) goto FortranFormat_accept;
        goto FortranFormat_loop;
    }
    if ((FortranFormat_n = FortranFormat_gindex[FortranFormat_m]) && (FortranFormat_n += FortranFormat_state) >= 0 &&
            FortranFormat_n <= YYTABLESIZE && FortranFormat_check[FortranFormat_n] == FortranFormat_state)
        FortranFormat_state = FortranFormat_table[FortranFormat_n];
    else
        FortranFormat_state = FortranFormat_dgoto[FortranFormat_m];
#if YYDEBUG
    if (FortranFormat_debug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *FortranFormat_ssp, FortranFormat_state);
#endif
    if (FortranFormat_ssp >= FortranFormat_sslim && FortranFormat_growstack())
    {
        goto FortranFormat_overflow;
    }
    *++FortranFormat_ssp = FortranFormat_state;
    *++FortranFormat_vsp = FortranFormat_val;
    goto FortranFormat_loop;

FortranFormat_overflow:
    FortranFormat_error(aParser,"yacc stack overflow");

FortranFormat_abort:
    return (1);

FortranFormat_accept:
    return (0);
}
