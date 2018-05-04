#ifndef Lib_Processor_h
#define Lib_Processor_h 

#include <string>
#include <vector>

#include <Lib/Value.h>
#include <Lib/Expression.h>
#include <Lib/ProcessorLexer.h>

namespace Lib {class IExpFunc;}
namespace Lib {class ExpFuncs;}

namespace Lib {

class Processor {
public:
  Processor(const std::string&,std::ostream&);
  virtual ~Processor();
  Processor(const Processor&);
  Processor& operator=(const Processor&);
public:
  void setScript(const std::string&);
  bool findVariables(std::vector<std::string>&);
  bool findFunctions(std::vector<std::string>&);
  bool compile();
  bool compile(const std::vector<std::string>&);
  bool execute(const std::vector<Lib::Value>&,Lib::Value&,std::string&);
  bool cut(const std::vector<Lib::Value>&,bool&,std::string&);
  void clear();
  void addFunction(IExpFunc*);
public:  // Internal :
  bool findNames() const;
  bool findFunctions() const;
  void addName(const std::string&);
  bool findVariable(const std::string&,unsigned int&);
  Lib::IExpFunc* findFunction(const std::string&,bool warn = true) const;
  ProcessorLexer& lexer();
  std::ostream& out() const;
  std::string error() const;
  std::vector<Expression*>& stack();
  void setResult(Expression*);
  void setStatus(bool);
private:
  std::string fString;
  std::ostream& f_out;
  bool fFindNames;
  bool fFindFunctions;
  std::vector<std::string> fNames;
  ProcessorLexer fLexer;
  Expression* fResult;
  bool fSuccess;
  std::vector<Expression*> fTmpExps;
  std::string fError;
  Lib::ExpFuncs* fFuncs;
};

}

#endif
