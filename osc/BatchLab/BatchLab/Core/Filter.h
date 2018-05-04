#ifndef BatchLab_Filter_h
#define BatchLab_Filter_h

// Inheritance :
#include <BatchLab/Core/Calculator.h>
#include <AIDA/IFilter.h>

namespace BatchLab {

class Filter 
:public Calculator
,public virtual AIDA::IFilter 
{
public: //AIDA/IFilter
  virtual bool initialize(AIDA::ITuple& aTuple) {
    return Calculator::initialize(aTuple);
  }
  virtual std::string expression() const {return fScript;}
  virtual bool accept() const {
    INLIB_SELF(Filter);
    if(!fScript.size()) return true;
    if(!self.getVariables()) return true; //FIXME : throw
    std::string serror;
    if(!fProcessor->execute(fVals,self.fResult,serror)) { //FIXME : throw
      f_out << "BatchLab::Filter::accept :"
          << " processor execution failed for " << inlib::sout(fScript) << "."
          << std::endl;
      f_out << serror << std::endl;
      return true;
    }
    if(fResult.type()!=Slash::Core::IValue::BOOL) {
      f_out << "BatchLab::Filter::accept :"
          << " result Lib::Variable not a BOOLEAN" 
          << " (" << fResult.stype() << ")"
          << " for " << inlib::sout(fScript)
          << std::endl;
      return true;
    }
    return fResult.get_bool();
  }
public:
  Filter(std::ostream& a_out,const std::string& aScript,AIDA::ITree& aTree)
  :Calculator(a_out,aScript,aTree){
    Lib::Debug::increment("BatchLab::Filter");
  }
  virtual ~Filter(){
    Lib::Debug::decrement("BatchLab::Filter");
  }
private:
  Filter(const Filter& aFrom)
  : AIDA::IFilter(),Calculator(aFrom){
    Lib::Debug::increment("BatchLab::Filter");
  }
  Filter& operator=(const Filter&) {return *this;}
};

}

#endif
