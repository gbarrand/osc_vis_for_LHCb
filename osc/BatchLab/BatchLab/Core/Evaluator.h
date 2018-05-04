#ifndef BatchLab_Evaluator_h
#define BatchLab_Evaluator_h

// Inheritance :
#include <BatchLab/Core/Calculator.h>
#include <AIDA/IEvaluator.h>

namespace BatchLab {

class Evaluator 
:public Calculator
,public virtual AIDA::IEvaluator 
{
public: //AIDA/IEvaluator
  virtual bool initialize(AIDA::ITuple& aTuple) {
    return Calculator::initialize(aTuple);
  }
  virtual std::string expression() const {return fScript;}
  virtual double evaluateDouble() const {
    INLIB_SELF(Evaluator);
    if(!self.getVariables()) return 0; //FIXME : throw
    std::string serror;
    if(!fProcessor->execute(fVals,self.fResult,serror)) { //FIXME : throw
      f_out << "BatchLab::Evaluator::evaluateDouble :"
          << " processor execution failed for " << inlib::sout(fScript) << "."
          << std::endl;
      f_out << serror << std::endl;
      return 0;
    }
    double value;
    if(!self.fResult.to_double(value)) {
      f_out << "BatchLab::Evaluator::evaluateDouble :"
          << " can't convert the result Lib::Value to a double"
          << " (" << fResult.stype() << ")"
          << " for " << inlib::sout(fScript)
          << std::endl;
      return 0;
    }
    return value;
  }
public:
  Evaluator(std::ostream& a_out,const std::string& aScript,AIDA::ITree& aTree)
  :Calculator(a_out,aScript,aTree){
    Lib::Debug::increment("BatchLab::Evaluator");
  }
  virtual ~Evaluator() {
    Lib::Debug::decrement("BatchLab::Evaluator");
  }
private:
  Evaluator(const Evaluator& aFrom)
  : AIDA::IEvaluator(),Calculator(aFrom){
    Lib::Debug::increment("BatchLab::Evaluator");
  }
  Evaluator& operator=(const Evaluator&){return *this;}
};

}

#endif
