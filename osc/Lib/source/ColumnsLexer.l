%{

// this :
#include <Lib/ColumnsLexer.h>

#include <Lib/Value.h>
#include <Lib/ColumnsToken.h>

#include <string.h>

#ifndef __APPLE__
extern "C" {extern int isatty (int ) throw();}
#endif

static  int yy_get_next_buffer(Lib::ColumnsLexer&);

extern int yylex(Lib::ColumnsLexer&);

#define YY_DECL int yylex(Lib::ColumnsLexer& aLexer)

#define _UNISTD_H_
#define YY_NEVER_INTERACTIVE 1

#undef YY_INPUT
#define YY_INPUT(aBuffer,aResult,aMaxSize) (aResult=aLexer.input(aBuffer,aMaxSize)) 

#define YY_NO_INPUT

%}

/* signus handled in by the parser :
number [-+]?([0-9]+|(([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][-+]?[0-9]+)?)
integer [0-9]+
*/
integer [0-9]+
hexaint 0x[0-9a-fA-F]+
number (([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][-+]?[0-9]+)?
name   [a-zA-Z_][a-zA-Z0-9_\-.]*
string (\"[^\"\n]*\")|('[^'\n]*')

%%


[\t ]+ /* ignore whitespace */;

{hexaint} { 
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::ColumnLexers::parse : "
                  << " hexaint : " << inlib::sout(yytext) 
                  << std::endl;
  }
  if(sscanf(yytext,"0x%lx",&(Columns_lval.fInteger))!=1) {
    Columns_lval.fInteger = 0;
  }
  return NUMBER_INTEGER;
}

{integer} { 
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::ColumnLexers::parse : "
                  << " integer : " << inlib::sout(yytext) 
                  << std::endl;
  }
  ;
  if(!inlib::to<unsigned long>(yytext,Columns_lval.fInteger)) {
    Columns_lval.fInteger = 0;
  }
  return NUMBER_INTEGER;
}

{number} {
  char* s;
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::ColumnLexers::parse : "
                  << " number : " << inlib::sout(yytext) 
                  << std::endl;
  }
  if(!inlib::to<double>(yytext,Columns_lval.fNumber)) {
    Columns_lval.fNumber = 0.;
  }
  return NUMBER_REAL;
}

true return T_TRUE;
TRUE return T_TRUE;
True return T_TRUE;
false return T_FALSE;
FALSE return T_FALSE;
False return T_FALSE;

or return T_OR;
OR return T_OR;
and return T_AND;
AND return T_AND;

short return T_SHORT;
int return T_INT;
long return T_INT64;
float return T_FLOAT;
double return T_DOUBLE;
boolean return T_BOOLEAN;
byte return T_BYTE;
char return T_CHAR;
string return T_STRING;
java.lang.String return T_JAVA_STRING;
ITuple return T_TUPLE;

{name} {
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::ColumnLexers::parse : "
                  << " name : " << inlib::sout(yytext) 
                  << std::endl;
  }
  aLexer.variables().push_back(yytext);
  Columns_lval.fIndex = aLexer.variables().size()-1;
  return T_NAME;
}

{string} {
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::ColumnLexers::parse : "
                  << " string : " << inlib::sout(yytext) 
                  << std::endl;
  }
  // Remove leading and trailing double quote :
  // "abcd"
  // 012345
  std::string s = yytext;
  aLexer.variables().push_back(s.substr(1,s.length()-2)); 
  int index = aLexer.variables().size()-1;
  Columns_lval.fIndex = index;
  return STRG;
}

. return yytext[0];

%%

Lib::ColumnsLexer::ColumnsLexer(std::ostream& a_out,int aVerboseLevel)
:fVerboseLevel(aVerboseLevel)
,fCurrent(0)
,fEnd(0)
,f_out(a_out)
{}

int Lib::ColumnsLexer::verboseLevel() const { return fVerboseLevel;}

Lib::ColumnsLexer& Lib::ColumnsLexer::operator=(const ColumnsLexer& aColumnsLexer){
  fVerboseLevel = aColumnsLexer.fVerboseLevel;
  // Reset other things :
  fString = "";
  fCurrent = 0;
  fEnd = 0;
  fVariables.clear();
  return *this;
}

void Lib::ColumnsLexer::set(const std::string& aString) {
  fString = aString;
  fCurrent = (char*)fString.c_str();
  fEnd = fCurrent + ::strlen(fCurrent);
}

std::ostream& Lib::ColumnsLexer::out() const { return f_out;}

std::vector<std::string>& Lib::ColumnsLexer::variables(){ return fVariables;}

std::string Lib::ColumnsLexer::variable(unsigned int aIndex) const {
  return fVariables[aIndex];
}

int Lib::ColumnsLexer::input(char* aBuffer,int aMaxSize) {
  if(aMaxSize<=0) return 0;
  int rest = fEnd - fCurrent;
  int n = aMaxSize>rest ? rest : aMaxSize;
  if(n>0) {
    ::memcpy(aBuffer,fCurrent,n);
    fCurrent += n;
  }
  return n;
}

int yywrap(){return 1;}

