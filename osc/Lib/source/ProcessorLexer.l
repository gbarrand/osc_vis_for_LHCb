%{

// this :
#include <Lib/ProcessorLexer.h>

// Lib :
namespace Lib {class Expression;}
#include <Lib/ProcessorTokens.h>

#include <inlib/sout>
#include <inlib/sto>
#include <inlib/math>

#include <cstring>

//#define DEBUG

#ifndef __APPLE__
extern "C" {extern int isatty (int ) throw();}
#endif

static  int yy_get_next_buffer(Lib::ProcessorLexer&);

extern int yylex(Lib::ProcessorLexer&);

#define YY_DECL int yylex(Lib::ProcessorLexer& aLexer)

#define _UNISTD_H_
#define YY_NEVER_INTERACTIVE 1

#undef YY_INPUT
#define YY_INPUT(aBuffer,aResult,aMaxSize) (aResult=aLexer.input(aBuffer,aMaxSize)) 

#define YY_NO_INPUT

#define NotFound (-1)

%}

/* signus handled in by the parser :
number [-+]?([0-9]+|(([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][-+]?[0-9]+)?)
integer [0-9]+
*/
integer ([0-9]+)([eE][+]?[0-9]+)?
hexaint 0x[0-9a-fA-F]+
number (([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][-+]?[0-9]+)?
name   [a-zA-Z_][a-zA-Z0-9_]*
string (\"[^\"\n]*\")|('[^'\n]*')

%%


[\t ]+ /* ignore whitespace */;

{hexaint} { 
#ifdef DEBUG
  aLexer.out() << "debug : Lexer.l : integer :" << yytext << "|" << std::endl;
#endif
  unsigned int i;
  if(sscanf(yytext,"0x%x",&i)!=1) {
    aLexer.out() << "Lib::ProcessorLexer :"
        << " problem converting " << inlib::sout(yytext) 
        << " to unsigned int."
        << std::endl;
    Processor_lval.fInteger = 0;
  } else {
    Processor_lval.fInteger = i;
  }
  return NUMBER_INTEGER;
}

{integer} { 
  char* s;
#ifdef DEBUG
  aLexer.out() << "debug : Lexer.l : integer :" << yytext << "|" << std::endl;
#endif
  unsigned int i,j;
  if( (sscanf(yytext,"%ue%u",&i,&j)==2)  ||
      (sscanf(yytext,"%ue+%u",&i,&j)==2) ||
      (sscanf(yytext,"%uE%u",&i,&j)==2)  ||
      (sscanf(yytext,"%uE+%u",&i,&j)==2) ){
    typedef unsigned int uint_t;
    Processor_lval.fInteger = i * inlib::power<uint_t>(10,j);
  } else if(sscanf(yytext,"%u",&i)==1) {
    Processor_lval.fInteger = i;
  } else {
    aLexer.out() << "Lib::ProcessorLexer :"
        << " problem converting " << inlib::sout(yytext) 
        << " to long."
        << std::endl;
    Processor_lval.fInteger = 0;
  }
  return NUMBER_INTEGER;
}

{number} {
  char* s;
#ifdef DEBUG
  aLexer.out() << "debug : Lexer.l : number :" << yytext << "|" << std::endl;
#endif
  if(!inlib::to<double>(yytext,Processor_lval.fNumber)) {
    aLexer.out() << "Lib::ProcessorLexer :"
        << " problem converting " << inlib::sout(yytext) 
        << " to double."
        << std::endl;
    Processor_lval.fNumber = 0.;
  }
  return NUMBER_REAL;
}


M_PI return T_PI;

true return T_TRUE;
TRUE return T_TRUE;
True return T_TRUE;
false return T_FALSE;
FALSE return T_FALSE;
False return T_FALSE;

PI return T_PI;
pi return T_PI;
or return T_OR;
OR return T_OR;
and return T_AND;
AND return T_AND;

{name} {
#ifdef DEBUG
  aLexer.out() << "debug : Lexer.l : name :" << yytext << "|" << std::endl;
#endif
  aLexer.variables().push_back(yytext);
  Processor_lval.fIndex = aLexer.variables().size()-1;
  return T_NAME;
}

{string} {
#ifdef DEBUG
  aLexer.out() << "debug : Lexer.l : string :" << yytext << "|" << std::endl;
#endif
  // Remove leading and trailing double quote :
  // "abcd"
  // 012345
  std::string s = yytext;
  aLexer.variables().push_back(s.substr(1,s.length()-2)); 
  int index = aLexer.variables().size()-1;
  Processor_lval.fIndex = index;
  return STRG;
}

. return yytext[0];

%%

Lib::ProcessorLexer::ProcessorLexer(std::ostream& a_out)
:fCurrent(0)
,fEnd(0)
,f_out(a_out)
{}

Lib::ProcessorLexer::ProcessorLexer(const Lib::ProcessorLexer& aFrom
)
:fCurrent(0)
,fEnd(0)
,f_out(aFrom.f_out)
{}

Lib::ProcessorLexer& Lib::ProcessorLexer::operator=(const ProcessorLexer& /*aProcessorLexer*/){
  // Reset other things :
  fString = "";
  fCurrent = 0;
  fEnd = 0;
  fVariables.clear();
  return *this;
}

void Lib::ProcessorLexer::set(const std::string& aString) {
  fString = aString;
  fCurrent = (char*)fString.c_str();
  fEnd = fCurrent + ::strlen(fCurrent);
}
std::ostream& Lib::ProcessorLexer::out() const {return f_out;}

std::vector<std::string>& Lib::ProcessorLexer::variables(){return fVariables;}

const std::string& Lib::ProcessorLexer::variable(unsigned int aIndex) const {
  return fVariables[aIndex];
}

int Lib::ProcessorLexer::input(char* aBuffer,int aMaxSize) {
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

