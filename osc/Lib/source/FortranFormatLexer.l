%{

// this :
#include <Lib/FortranFormatLexer.h>

#include <Lib/FortranFormatToken.h>

#include <inlib/sout>

#ifndef __APPLE__
extern "C" {extern int isatty (int ) throw();}
#endif

static  int yy_get_next_buffer(Lib::FortranFormatLexer&);

extern int yylex(Lib::FortranFormatLexer&);

#define YY_DECL int yylex(Lib::FortranFormatLexer& aLexer)

#define _UNISTD_H_
#define YY_NEVER_INTERACTIVE 1

#undef YY_INPUT
#define YY_INPUT(aBuffer,aResult,aMaxSize) (aResult=aLexer.input(aBuffer,aMaxSize)) 

#define YY_NO_INPUT

%}

integer [0-9]+

exponent E[0-9]+

%%

[\t ]+ /* ignore whitespace */;

{integer} { 
  char* s;
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::FortranFormatLexer::parse :"
        << " integer : " << inlib::sout(yytext) 
        << std::endl;
  }
  FortranFormat_lval.fInteger = ::strtol(yytext,&s,10);
  if(s==yytext) {
    FortranFormat_lval.fInteger = 0;
  }
  return NUMBER_INTEGER;
}

{exponent} { 
  char* s;
  if(aLexer.verboseLevel()) {
    aLexer.out() << "Lib::FortranFormatLexer::parse :"
        << " exponent : " << inlib::sout(yytext) 
        << std::endl;
  }
  FortranFormat_lval.fInteger = ::strtol(yytext+1,&s,10);
  if(s==yytext) {
    FortranFormat_lval.fInteger = 0;
  }
  return NUMBER_EXPONENT;
}

X return T_X;
x return T_X;
I return T_I;
i return T_I;
F return T_F;
f return T_F;
E return T_E;
e return T_E;
G return T_G;
g return T_G;
A return T_A;
a return T_A;
L return T_L;
l return T_L;
\/ return T_SLASH;

. return yytext[0];

%%

Lib::FortranFormatLexer::FortranFormatLexer(std::ostream& a_out,int aVerboseLevel)
:fVerboseLevel(aVerboseLevel)
,fCurrent(0)
,fEnd(0)
,f_out(a_out)
{}

int Lib::FortranFormatLexer::verboseLevel() const {return fVerboseLevel;}

// private.
Lib::FortranFormatLexer::FortranFormatLexer(const FortranFormatLexer& aFrom)
:f_out(aFrom.f_out)
{}
Lib::FortranFormatLexer& Lib::FortranFormatLexer::operator=(const FortranFormatLexer&){return *this;}


void Lib::FortranFormatLexer::set(const std::string& aString) {
  fString = aString;
  fCurrent = (char*)fString.c_str();
  fEnd = fCurrent + ::strlen(fCurrent);
}

std::ostream& Lib::FortranFormatLexer::out() const {return f_out;}

int Lib::FortranFormatLexer::input(char* aBuffer,int aMaxSize) {
  if(aMaxSize<=0) return 0;
  int rest = fEnd - fCurrent;
  int n = aMaxSize>rest ? rest : aMaxSize;
  if(n>0) {
    ::memcpy(aBuffer,fCurrent,n);
    fCurrent += n;
  }
  return n;
}

int yywrap() {return 1;}


