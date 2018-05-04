%{

// this :
#include <Lib/Processor.h>

#include <Lib/ProcessorLexer.h>
#include <Lib/ExpFunc.h>

#include <inlib/vmanip>

extern int yylex(Lib::ProcessorLexer&);
static int yyparse(void);
static void yyerror(Lib::Processor&,const char*); 

#define NotFound (-1)

//#define DEBUG

%}

%union {
  unsigned int fIndex;
  unsigned int fInteger;
  double fNumber;
  Lib::Expression* fExpression;
}

%token <fIndex> STRG
%token <fIndex> T_NAME
%token <fInteger> NUMBER_INTEGER
%token <fNumber> NUMBER_REAL 

%token T_PI T_TRUE T_FALSE
%token T_AND T_OR

%type <fExpression> comparisons comparison expression 

%left '-' '+'
%left '/' '*'
%nonassoc UMINUS

%%

result 
: error {
  // To reset lex and yacc in case of error.
} | comparisons {
  if(aParser.findNames()||aParser.findFunctions()) {
    aParser.setResult(0);
  } else {
    aParser.setResult($1);
    aParser.stack().pop_back();
  }
} | expression {
  if(aParser.findNames()||aParser.findFunctions()) {
    aParser.setResult(0);
  } else {
    aParser.setResult($1);
    aParser.stack().pop_back();
  }
};

comparisons 
: '!' comparisons {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : ! cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::NOT,$2);
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | comparisons '&' '&' comparisons {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps && cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::AND,$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | comparisons T_AND comparisons {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps && cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::AND,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | comparisons '|' '|' comparisons {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps || cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::OR,$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | comparisons T_OR comparisons {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cps || cps" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::OR,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | '(' comparisons ')'  {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : (cps)" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::ASSIGN,$2);
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | comparison {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : cp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::ASSIGN,$1);
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
};

comparison 
: expression '>' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp > exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::GT,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '>' '=' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp >= exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::GE,$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '<' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp < exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::LT,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '<' '=' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp <= exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::LE,$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '=' '=' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp == exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::EQ,$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '!' '=' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp != exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::NE,$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
};

expression 
: expression '+' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp + exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::ADD,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '-' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp - exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::SUB,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '*' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp * exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::MUL,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '*' '*' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp ** exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction("pow"),$1,$4);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | expression '/' expression {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : exp / exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::DIV,$1,$3);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | '-' expression %prec UMINUS { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : - exp" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::MINUS,$2);
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | '(' expression ')' { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : (exp)" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::ASSIGN,$2);
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }

} | T_NAME '(' expression ')' { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_COS" << std::endl;
#endif
  if(aParser.findNames()) {
    $$ = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       $3);
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | T_NAME '(' expression ',' expression ')' { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_POW" << std::endl;
#endif
  if(aParser.findNames()) {
    $$ = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);    
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       $3,$5);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | T_NAME '(' expression ',' expression ',' expression ')' {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_EXPO" << std::endl;
#endif
  if(aParser.findNames()) {
    $$ = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);    
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       $3,$5,$7);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | T_NAME '(' expression ',' expression ',' expression ',' expression ')' {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_GAUSS" << std::endl;
#endif
  if(aParser.findNames()) {
    $$ = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);    
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       $3,$5,$7,$9);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | T_NAME '(' expression ',' expression ',' expression ',' expression ',' expression ')' {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_POL3" << std::endl;
#endif
  if(aParser.findNames()) {
    $$ = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       $3,$5,$7,$9,$11);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }
} | T_NAME '(' expression ',' expression ',' expression ',' expression ',' expression ',' expression ')' {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_POL4" << std::endl;
#endif
  if(aParser.findNames()) {
    $$ = 0;
  } else if(aParser.findFunctions()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);    
    $$ = new Lib::Expression
      (Lib::Expression::FUNC,aParser.findFunction(var),
       $3,$5,$7,$9,$11,$13);
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().pop_back();
    aParser.stack().push_back($$);
  }

} | T_PI { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_PI" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::PI);
    aParser.stack().push_back($$);
  }
} | T_TRUE { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_TRUE" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::BOOL_TRUE);
    aParser.stack().push_back($$);
  }
} | T_FALSE { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_FALSE" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::BOOL_FALSE);
    aParser.stack().push_back($$);
  }
} | NUMBER_INTEGER { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : NUMBER_INTEGER" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression
      (Lib::Expression::UNSIGNED_INTEGER,(unsigned int)$1);
    aParser.stack().push_back($$);
  }
} | NUMBER_REAL { 
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : NUMBER_REAL" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::REAL,$1);
    aParser.stack().push_back($$);
  }
} | T_NAME {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : T_NAME" << std::endl;
#endif
  if(aParser.findNames()) {
    aParser.addName(aParser.lexer().variable($1));
    $$ = 0;
  } else if(aParser.findFunctions()) {
    $$ = 0;
  } else {
    std::string var = aParser.lexer().variable($1);
    unsigned int index;
    int ivar = NotFound;
    if(aParser.findVariable(var,index)) ivar = int(index);
    $$ = new Lib::Expression(Lib::Expression::NAME,var,ivar);
    aParser.stack().push_back($$);
  }
} | STRG {
#ifdef DEBUG
  aParser.out() << "debug : Processor.y : STRG" << std::endl;
#endif
  if(aParser.findNames()||aParser.findFunctions()) {
    $$ = 0;
  } else {
    $$ = new Lib::Expression(Lib::Expression::STRING,
                             aParser.lexer().variable($1));
    aParser.stack().push_back($$);
  }
};

%%

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
  yyparse(*this);
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
  yyparse(*this);
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
  yyparse(*this);
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

static void yyerror(Lib::Processor& aParser,const char* aString) {
  aParser.out() << "Processor : " << aString << std::endl;
  aParser.setStatus(false);
}
