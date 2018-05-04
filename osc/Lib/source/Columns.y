%{

// this :
#include <Lib/Columns.h>

#include <Lib/ColumnsLexer.h>

extern int yylex(Lib::ColumnsLexer&);
static int yyparse(void);
static void yyerror(Lib::Columns&,const char*); 

#define NotFound (-1)

%}

%union {
  int fIndex;
  unsigned long fInteger;
  double fNumber;
  Slash::Core::IValue::Type fType;
  Lib::Value* fVariable;
}

%token <fIndex> STRG
%token <fIndex> T_NAME
%token <fInteger> NUMBER_INTEGER
%token <fNumber> NUMBER_REAL 

%token T_TRUE T_FALSE
%token T_AND T_OR

%token T_SHORT T_INT T_INT64 T_FLOAT T_DOUBLE T_BOOLEAN T_BYTE T_CHAR T_STRING
%token T_JAVA_STRING T_TUPLE

%type <fType> type
%type <fVariable> value declaration

%left '-' '+'
%left '/' '*'
%nonassoc UMINUS

%%

result 
: error {
  // To reset lex and yacc in case of error.
} | declarations {
};

declarations 
: declaration {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : declarations : declaration : " 
        << $1->label() << std::endl;
  }
  aParser.fStack.push_front(*($1));
  delete $1;
  aParser.fTmpVars.pop_back();
} 
| declaration ',' declarations {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : dec , decls : " << $1->label() << std::endl;
  }
  aParser.fStack.push_front(*($1));
  delete $1;
  aParser.fTmpVars.pop_back();
}
| declaration ',' {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : dec , decls : " << $1->label() << std::endl;
  }
  aParser.fStack.push_front(*($1));
  delete $1;
  aParser.fTmpVars.pop_back();
};

declaration 
: T_TUPLE T_NAME '=' '{' declarations '}' {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_TUPLE T_NAME = {decs} : " 
        << aParser.lexer().variable($2) << std::endl;
  }
  Lib::Value* v = 
    new Lib::Value((void*)new std::list<Lib::Value>(aParser.fStack));
  v->setLabel(aParser.lexer().variable($2));
  aParser.fStack.clear();
  aParser.fCurrentType = Slash::Core::IValue::NONE; //FIXME
  $$ = v;
  aParser.fTmpVars.push_back($$);
} 
| T_NAME {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_NAME : " 
        << aParser.lexer().variable($1) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable($1));
  v->setType(aParser.fCurrentType);
  $$ = v;
  aParser.fTmpVars.push_back($$);
} 
| type T_NAME {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : type T_NAME : " 
        << aParser.lexer().variable($2) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable($2));
  v->setType($1);
  aParser.fCurrentType = $1;
  $$ = v;
  aParser.fTmpVars.push_back($$);
} 
| type '[' ']' T_NAME {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : type [] T_NAME : " 
        << aParser.lexer().variable($4) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable($4));
  Slash::Core::IValue::Type atype;
  if(!Lib::Value::arrayType($1,atype)) {
    aParser.out() << "Lib::Columns::parse : type [] T_NAME :" 
        << " problem to get array type of " << Lib::Value::stype($1)
        << std::endl;
  }
  v->setType(atype);
  aParser.fCurrentType = atype;
  $$ = v;
  aParser.fTmpVars.push_back($$);
} 
| T_NAME '=' value {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_NAME = value : " 
        << aParser.lexer().variable($1) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable($1));
  v->setType(aParser.fCurrentType);
  std::string serror;
  if(!Lib::Value::assign(*v,*($3),aParser.fError)) 
    aParser.fSuccess = false;
  delete $3;
  $$ = v;
  aParser.fTmpVars.pop_back();
  aParser.fTmpVars.push_back($$);
} 
| type T_NAME '=' value {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : type T_NAME = value : " 
        << aParser.lexer().variable($2) << std::endl;
  }
  Lib::Value* v = new Lib::Value();
  v->setLabel(aParser.lexer().variable($2));
  v->setType($1);
  if(!Lib::Value::assign(*v,*($4),aParser.fError)) 
    aParser.fSuccess = false;
  delete $4;
  aParser.fCurrentType = $1;
  $$ = v;
  aParser.fTmpVars.pop_back();
  aParser.fTmpVars.push_back($$);
}; 


type  
: T_SHORT { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_SHORT" << std::endl;
  }
  $$ = Slash::Core::IValue::SHORT;
} 
| T_INT { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_INT" << std::endl;
  }
  $$ = Slash::Core::IValue::INT;
} 
| T_INT64 { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_INT64" << std::endl;
  }
  $$ = Slash::Core::IValue::INT64;
} 
| T_FLOAT { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_FLOAT" << std::endl;
  }
  $$ = Slash::Core::IValue::FLOAT;
} 
| T_DOUBLE {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_DOUBLE" << std::endl;
  }
  $$ = Slash::Core::IValue::DOUBLE;
} 
| T_BOOLEAN {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_BOOLEAN" << std::endl;
  }
  $$ = Slash::Core::IValue::BOOL;
} 
| T_BYTE {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_BYTE" << std::endl;
  }
  $$ = Slash::Core::IValue::UNSIGNED_CHAR;
} 
| T_CHAR {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_CHAR" << std::endl;
  }
  $$ = Slash::Core::IValue::CHAR;
} 
| T_STRING {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_STRING" << std::endl;
  }
  $$ = Slash::Core::IValue::STRING;
} 
| T_JAVA_STRING {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_JAVA_STRING" << std::endl;
  }
  $$ = Slash::Core::IValue::STRING;
};

value 
: T_TRUE { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_TRUE" << std::endl;
  }
  $$ = new Lib::Value(true);
  aParser.fTmpVars.push_back($$);
} 
| T_FALSE { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : T_FALSE" << std::endl;
  }
  $$ = new Lib::Value(false);
  aParser.fTmpVars.push_back($$);
} 
| '-' NUMBER_INTEGER { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_INTEGER" << std::endl;
  }
  $$ = new Lib::Value(-(int)$2);
  aParser.fTmpVars.push_back($$);
} 
| NUMBER_INTEGER { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_INTEGER" << std::endl;
  }
  $$ = new Lib::Value((int)$1);
  aParser.fTmpVars.push_back($$);
} 
| NUMBER_REAL { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_REAL" << std::endl;
  }
  $$ = new Lib::Value((double)$1);
  aParser.fTmpVars.push_back($$);
} 
| '-' NUMBER_REAL { 
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : NUMBER_REAL" << std::endl;
  }
  $$ = new Lib::Value((double)(-1. * $2));
  aParser.fTmpVars.push_back($$);
} 
| STRG {
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : STRG" << std::endl;
  }
  $$ = new Lib::Value(aParser.lexer().variable($1));
  aParser.fTmpVars.push_back($$);
} 
| T_NAME {
  // For char.
  if(aParser.fVerboseLevel) {
    aParser.out() << "Lib::Columns::parse : STRG" << std::endl;
  }
  $$ = new Lib::Value(aParser.lexer().variable($1));
  aParser.fTmpVars.push_back($$);
};

%%

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
  yyparse(*this);
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

static void yyerror(Lib::Columns& aParser,const char* aString) {
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
