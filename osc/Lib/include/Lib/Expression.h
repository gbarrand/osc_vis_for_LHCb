#ifndef Lib_Expression_h
#define Lib_Expression_h

#include <string>
#include <vector>
#include <ostream>

#include <inlib/math>
#include <inlib/smatch>

#include <Lib/Value.h>
#include <Lib/Debug.h>
#include <Lib/IExpFunc.h>

#include <cmath>

namespace Lib {class IExpFunc;}

namespace Lib {

class Expression {
public:
  enum eType {
    GT = 1,
    GE = 2,
    LT = 3,
    LE = 4,
    EQ = 5,
    NE = 6,
    ADD = 7,
    MUL = 8,
    SUB = 9,
    DIV = 10,
    AND = 11,
    OR = 12,
    ASSIGN = 13,
    MINUS = 14,
    UNSIGNED_INTEGER = 15,
    REAL = 16,
    NAME = 17,
    STRING = 18,
    PI = 19,
    FUNC = 20,
    BOOL_TRUE = 21,
    BOOL_FALSE = 22,
    NOT = 23
  };
public:
  Expression(eType aType)
  :fType(aType),fFunction(0),fIndex(NotFound)
  ,fA(0),fB(0),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Expression* aA)
  :fType(aType),fFunction(0),fIndex(NotFound)
  ,fA(aA),fB(0),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Expression* aA,Expression* aB)
  :fType(aType),fFunction(0),fIndex(NotFound)
  ,fA(aA),fB(aB),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Lib::IExpFunc* aFunction,Expression* aA)
  :fType(aType),fFunction(aFunction),fIndex(NotFound)
  ,fA(aA),fB(0),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Lib::IExpFunc* aFunction,
             Expression* aA,Expression* aB)
  :fType(aType),fFunction(aFunction),fIndex(NotFound)
  ,fA(aA),fB(aB),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Lib::IExpFunc* aFunction,
             Expression* aA,Expression* aB,Expression* aC)
  :fType(aType),fFunction(aFunction),fIndex(NotFound)
  ,fA(aA),fB(aB),fC(aC),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Lib::IExpFunc* aFunction,
             Expression* aA,Expression* aB,Expression* aC,Expression* aD)
  :fType(aType),fFunction(aFunction),fIndex(NotFound)
  ,fA(aA),fB(aB),fC(aC),fD(aD),fE(0),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Lib::IExpFunc* aFunction,
             Expression* aA,Expression* aB,Expression* aC,
             Expression* aD,Expression* aE)
  :fType(aType),fFunction(aFunction),fIndex(NotFound)
  ,fA(aA),fB(aB),fC(aC),fD(aD),fE(aE),fF(0){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,Lib::IExpFunc* aFunction,
             Expression* aA,Expression* aB,Expression* aC,
             Expression* aD,Expression* aE,Expression* aF)
  :fType(aType),fFunction(aFunction),fIndex(NotFound)
  ,fA(aA),fB(aB),fC(aC),fD(aD),fE(aE),fF(aF){
    Debug::increment("Lib::Expression");
  }

  Expression(eType aType,unsigned int aNumber)
  :fType(aType),fFunction(0),fIndex(NotFound)
  ,fA(0),fB(0),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
    fVariable.set(aNumber);
  }

  Expression(eType aType,double aNumber)
  :fType(aType),fFunction(0),fIndex(NotFound)
  ,fA(0),fB(0),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
    fVariable.set(aNumber);
  }

  Expression(eType aType,const std::string& aString)
  :fType(aType),fFunction(0),fIndex(NotFound)
  ,fA(0),fB(0),fC(0),fD(0),fE(0),fF(0){
    Debug::increment("Lib::Expression");
    fVariable.set(aString);
  }

  Expression(eType aType,const std::string& aName,int aIndex)
  :fType(aType),fFunction(0),fName(aName),fIndex(aIndex)
  ,fA(0),fB(0),fC(0),fD(0),fE(0),fF(0){
    // aName needed to not confuse with the constructor (eType,unsigned int).
    Debug::increment("Lib::Expression");
  }

  virtual ~Expression() {
    delete fA;
    delete fB;
    delete fC;
    delete fD;
    delete fE;
    delete fF;
    Debug::decrement("Lib::Expression");
  }
public:
  void print(std::ostream& a_out) {
    a_out << "Type : " << this << " " << (int)fType << std::endl;
    //if(!fVariable.is_none()) fVariable.print(a_out);
    if(fA) fA->print(a_out);
    if(fB) fB->print(a_out);
  }

  bool execute(const std::vector<Lib::Value>& aInput,
               std::string& aError){ //Inited by the caller to optimize.
    //FIXME : handle ret values of execute().
    switch(fType) {
    case GT:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::if_gt(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case GE:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::if_ge(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case LT:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::if_lt(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case LE:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::if_le(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case EQ:
      if(!fA || !fB) break;
     {if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      Lib::Value& va = fA->ret();
      Lib::Value& vb = fB->ret();
      if( (fA->fType==NAME) && 
          (va.type()==Slash::Core::IValue::STRING) &&
          (vb.type()==Slash::Core::IValue::STRING) ){
        fReturn.set(inlib::match(va.get_string(),vb.get_string()));
        return true;
      } else {
        fReturn = va;
       {if(!Lib::Value::if_eq(fReturn,vb,aError)) {
          fReturn.setNone();
          return false;
        }}
        return true;
      }}
    case NE:
      if(!fA || !fB) break;
     {if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      Lib::Value& va = fA->ret();
      Lib::Value& vb = fB->ret();
      if( (fA->fType==NAME) && 
          (va.type()==Slash::Core::IValue::STRING) &&
          (vb.type()==Slash::Core::IValue::STRING) ){
        fReturn.set(inlib::match(va.get_string(),
                                  vb.get_string())?false:true);
        return true;
      } else {
        fReturn = va;
       {if(!Lib::Value::if_ne(fReturn,vb,aError)) {
          fReturn.setNone();
          return false;
        }}
        return true;
      }}
    case AND:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::if_and(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case OR:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::if_or(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case NOT:
      if(!fA) break;
      if(!EXEC(fA,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::do_not(fReturn,aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case ASSIGN:
      if(!fA) break;
      if(!EXEC(fA,aInput,aError)) return false;
      fReturn = fA->ret(); //FIXME : or fReturn.assign(fA->ret()); ???
      return true;
    case ADD:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::add(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case MUL:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::multiply(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case SUB:
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::subtract(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case DIV:{
      if(!fA || !fB) break;
      if(!EXEC(fA,aInput,aError)) return false;
      if(!EXEC(fB,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::divide(fReturn,fB->ret(),aError)) {
        fReturn.setNone();
        return false;
      }}
      return true;
    case MINUS:
      if(!fA) break;
      if(!EXEC(fA,aInput,aError)) return false;
      fReturn = fA->ret();
     {if(!Lib::Value::minus(fReturn,aError)) {
       fReturn.setNone();
       return false;
      }}
      return true;
    case NAME:
      if( (fIndex<0) || (fIndex>=(int)aInput.size()) ) {
        aError = std::string("Expression::execute : index out of range")
    	  + " " + inlib::to<int>((int)fIndex)
  	  + " range [0," + inlib::to<int>((int)aInput.size())
          + "] for variable "+fName+".";
        fReturn.setNone();
        return false;
      }
      fReturn = aInput[fIndex];
      return true;
    case UNSIGNED_INTEGER:
    case REAL:
    case STRING:
      fReturn = fVariable;
      return true;
    case PI:
      fReturn.set(inlib::pi());
      return true;
    case BOOL_TRUE:
      fReturn.set(true);
      return true;
    case BOOL_FALSE:
      fReturn.set(false);
      return true;
    case FUNC:
      if(!fA) break;
      if(!fFunction) {
        aError = std::string("Expression::execute : null function");
        fReturn.setNone();
        return false;
      }
     {std::vector<Lib::Value> args;
  
      if(!EXEC(fA,aInput,aError)) return false;
      args.push_back(fA->ret());
  
      unsigned int argn = fFunction->numberOfArguments();
      if(argn==1) {
      } else if(argn==2) {
        if(!fB) break;
        if(!EXEC(fB,aInput,aError)) return false;
        args.push_back(fB->ret());
      } else if(argn==3) {
        if(!fB || !fC) break;
        if(!EXEC(fB,aInput,aError)) return false;
        if(!EXEC(fC,aInput,aError)) return false;
        args.push_back(fB->ret());
        args.push_back(fC->ret());
      } else if(argn==4) {
        if(!fB || !fC || !fD) break;
        if(!EXEC(fB,aInput,aError)) return false;
        if(!EXEC(fC,aInput,aError)) return false;
        if(!EXEC(fD,aInput,aError)) return false;
        args.push_back(fB->ret());
        args.push_back(fC->ret());
        args.push_back(fD->ret());
      } else if(argn==5) {
        if(!fB || !fC || !fD || !fE) break;
        if(!EXEC(fB,aInput,aError)) return false;
        if(!EXEC(fC,aInput,aError)) return false;
        if(!EXEC(fD,aInput,aError)) return false;
        if(!EXEC(fE,aInput,aError)) return false;
        args.push_back(fB->ret());
        args.push_back(fC->ret());
        args.push_back(fD->ret());
        args.push_back(fE->ret());
      } else if(argn==6) {
        if(!fB || !fC || !fD || !fE || !fF) break;
        if(!EXEC(fB,aInput,aError)) return false;
        if(!EXEC(fC,aInput,aError)) return false;
        if(!EXEC(fD,aInput,aError)) return false;
        if(!EXEC(fE,aInput,aError)) return false;
        if(!EXEC(fF,aInput,aError)) return false;
        args.push_back(fB->ret());
        args.push_back(fC->ret());
        args.push_back(fD->ret());
        args.push_back(fE->ret());
        args.push_back(fF->ret());
      }
      return fFunction->eval(args,fReturn,aError);
      }
    }
    }
    aError = std::string("Expression::execute : Can't apply on type ")
      + inlib::to<int>((int)fType) + " : bad return value.";
    fReturn.setNone();
    return false;
  }
  
  Lib::Value& ret() {return fReturn;}
private:
  static const int NotFound = -1;
private:
  bool EXEC(Expression* aExpression,
            const std::vector<Lib::Value>& aInput,
            std::string& aError){ //Inited by the caller to optimize.
    if(!aExpression->execute(aInput,aError)) {
      fReturn.setNone();
      return false;
    }
    return true;
  }
private:
  eType fType;
  Lib::IExpFunc* fFunction;
  Lib::Value fVariable;
  Lib::Value fReturn;
  std::string fName;
  int fIndex;
  //Below, can't have objects of same class than the class.
  Expression* fA; 
  Expression* fB;
  Expression* fC;
  Expression* fD;
  Expression* fE;
  Expression* fF;
};

}

#endif



