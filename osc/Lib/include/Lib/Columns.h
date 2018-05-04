#ifndef Lib_Columns_h
#define Lib_Columns_h 

#include <string>
#include <list>
#include <ostream>

#include <Lib/Value.h>
#include <Lib/ColumnsLexer.h>

namespace Lib {

class Columns {
public:
  Columns(const std::string&,std::ostream&,int = 0);
  virtual ~Columns();
private:
  Columns(const Columns& a_from):f_out(a_from.f_out),fLexer(a_from.f_out){}
  Columns& operator=(const Columns&);
public:
  void setScript(const std::string&);
  bool findVariables();
  std::list<Lib::Value> result() const;
  void clear(); 
  std::string script() const;
  bool isSuccess() const;
public:  // Internal :
  ColumnsLexer& lexer();
  std::ostream& out() const;
public:
  static void deleteColumns(std::list<Lib::Value>&);
  static void copyColumns(const std::list<Lib::Value>&,
                          std::list<Lib::Value>&);

  static void dumpColumns(const std::list<Lib::Value>& aVars,
                          std::ostream& a_out,
                          const std::string& a_margin = "") {
    std::list<Lib::Value>::const_iterator it;
    for(it=aVars.begin();it!=aVars.end();++it) {
      if((*it).type()==Lib::Value::VOID_STAR) {
        a_out << a_margin
              << " ---- ITuple : " << (*it).label() << " : begin "
              << std::endl;
        std::list<Lib::Value>* vars = 
          (std::list<Lib::Value>*)(*it).get_void_star();
        dumpColumns(*vars,a_out,a_margin+" ");
        a_out << a_margin
              << " ---- ITuple : " << (*it).label() << " : end "
              << std::endl;
      } else {
        //(*it).print(a_out);
        a_out << a_margin
              << (*it).stype() << " : " 
              << (*it).label() << " : " 
              << Lib::Value::toString(*it)
              << std::endl;
      }
    }
  }
public:
  int fVerboseLevel;
  std::string fString;
  std::ostream& f_out;
  std::list<Lib::Value> fStack;
  ColumnsLexer fLexer;
  bool fSuccess;
  Slash::Core::IValue::Type fCurrentType;
  std::vector<Lib::Value*> fTmpVars;
  std::string fError;
};

}

#endif
