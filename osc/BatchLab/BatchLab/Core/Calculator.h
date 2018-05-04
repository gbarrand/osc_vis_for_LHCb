#ifndef BatchLab_Calculator_h
#define BatchLab_Calculator_h

#include <Lib/ExpFunc.h>
#include <AIDA/IFunction.h>
#include <AIDA/ITree.h>
#include <BatchLab/Core/BaseTuple.h>
#include <Lib/Processor.h>
 
namespace BatchLab {

class Calculator {
public:
  //virtual const std::string& label() const;
  Calculator(std::ostream& a_out,const std::string& aScript,AIDA::ITree& aTree)
  :f_out(a_out),fTree(aTree),fTuple(0)
  ,fBaseTuple(0),fProcessor(0),fScript(aScript) {
    Lib::Debug::increment("BatchLab::Calculator");
  }
  virtual ~Calculator(){
    delete fProcessor;
    Lib::Debug::decrement("BatchLab::Calculator");
  }
protected:
  Calculator(const Calculator& aFrom)
  :f_out(aFrom.f_out),fTree(aFrom.fTree),fTuple(0)
  ,fBaseTuple(0),fProcessor(0),fScript(aFrom.fScript){
    Lib::Debug::increment("BatchLab::Calculator");
  }
  Calculator& operator=(const Calculator&) {return *this;}
protected:
  bool initialize(AIDA::ITuple& aTuple){
    fTuple = 0;
    delete fProcessor;
    fProcessor = 0;
    fNames.clear();
    fIndices.clear();
    fBaseTuple = 0;
    fBaseCols.clear();
    fVals.clear();
  
    fProcessor = new Lib::Processor(fScript,f_out);
  
    // Get cut variables :
    if(!fProcessor->findVariables(fNames)) {
      f_out << "BatchLab::Calculator::initialize : "
          << " findVariables failed." 
          << std::endl;
      fNames.clear();
      delete fProcessor;
      fProcessor = 0;
      return false;    
    }
  
    // Get functions names :
   {std::vector<std::string> funcs;
    if(!fProcessor->findFunctions(funcs)) {
      f_out << "BatchLab::Calculator::initialize : "
          << " findFunctions failed." 
          << std::endl;
      fNames.clear();
      delete fProcessor;
      fProcessor = 0;
      return false;    
    }
   {std::vector<std::string>::const_iterator it;
    for(it=funcs.begin();it!=funcs.end();++it) {
      if(fProcessor->findFunction(*it,false)) continue;
      AIDA::IManagedObject* bo = fTree.find(*it);
      //Slash::Store::IObject* bo = stree->findObject(*it);
      if(!bo) {
        f_out << "BatchLab::Calculator::initialize : "
            << " object " << inlib::sout(*it) 
            << " not found in tree with store "
            << inlib::sout(fTree.storeName())
            << std::endl;
        fNames.clear();
        delete fProcessor;
        fProcessor = 0;
        return false;    
      }
      AIDA::IFunction* func = INLIB_CAST(*bo,AIDA::IFunction);
      if(!func) {
        f_out << "BatchLab::Calculator::initialize : "
            << " object " << inlib::sout(*it) 
            << " not an AIDA::IFunction."
            << std::endl;
        fNames.clear();
        delete fProcessor;
        fProcessor = 0;
        return false;    
      }
      fProcessor->addFunction(new ExpFunc_AIDA_FUNC(*it,*func));
    }}
    }
  
    if(!fProcessor->compile(fNames)) {
      f_out << "BatchLab::Calculator::initialize : "
          << " compilation failed." 
          << std::endl;
      fNames.clear();
      delete fProcessor;
      fProcessor = 0;
      return false;    
    }
  
    const int NotFound = -1;
  
    unsigned int number = fNames.size(); //Could be zero.
    fIndices.resize(number);
    for(unsigned int count=0;count<number;count++) {
      //printf("debug : variable \"%s\" %lu\n",fNames[count].c_str(),&aTuple);
      int index = aTuple.findColumn(fNames[count]);
      if(index==NotFound) {
        f_out << "BatchLab::Calculator::initialize : "
            << inlib::sout(fNames[count]) 
            << " not a column of tuple with title " 
            << inlib::sout(aTuple.title()) 
            << std::endl;
        delete fProcessor;
        fProcessor = 0;
        fNames.clear();
        fIndices.clear();
        fBaseTuple = 0;
        fBaseCols.clear();
        fVals.clear();
        // Dump column names :
       {f_out << "BatchLab::Calculator::initialize :"
              << " columns are :"
              << std::endl;
        for(unsigned int count=0;count<number;count++) {
          f_out << inlib::sout(aTuple.columnName(count)) << std::endl;
        }}
        return false;
      } 
      fIndices[count] = index;
    }
    fTuple = &aTuple;
  
    fVals.resize(number);
  
    // To optimize the evaluateDouble :
    if(!fTuple->cast("BatchLab::ChainedTuple")) {
      fBaseTuple = INLIB_CONST_CAST(*fTuple,BatchLab::BaseTuple);
      if(fBaseTuple) {
        fBaseCols.resize(number);
        for(unsigned int count=0;count<number;count++) {
          fBaseCols[count] = fBaseTuple->findColumn1(fIndices[count]);
        }
      }
    }
  
    return true;
  }

  bool getVariables(){
    if(!fProcessor) return false;
    unsigned int number = fNames.size(); //Could be zero.  
    if(fBaseTuple) {  
      // Faster :
      int irow = fBaseTuple->rowIndex();  
      for(unsigned int count=0;count<number;count++) {
        BaseColumn* column = fBaseCols[count];
        if(!column->value(irow,fVals[count])) {
          f_out << "BatchLab::Calculator::evaluateDouble : "
              << " can't get Variable value for column " 
              << inlib::sout(column->name()) << "."
              << std::endl;
          return false;
        }
      }  
    } else {  
      // Through AIDA::ITuple only but more slower :
      for(unsigned int count=0;count<number;count++) {
        int icol = fIndices[count];
        std::string type = fTuple->columnType(icol);
        if(type=="float") {
          fVals[count].set(fTuple->getFloat(icol));
        } else if(type=="double") {
          fVals[count].set(fTuple->getDouble(icol));
        } else if(type=="char") {
          fVals[count].set(fTuple->getChar(icol));
        } else if(type=="short") {
          fVals[count].set(fTuple->getShort(icol));
        } else if(type=="int") {
          fVals[count].set(fTuple->getInt(icol));
        } else if(type=="boolean") {
          fVals[count].set(fTuple->getBoolean(icol));
        } else {
          f_out << "BatchLab::Calculator::evaluateDouble : "
              << " make no sense on column type " << type << "."
              << std::endl;
        }
      }
    }
    return true;
  }

protected:
  std::ostream& f_out;
private:
  AIDA::ITree& fTree; //to find functions.
  const AIDA::ITuple* fTuple;
  std::vector<std::string> fNames;
  std::vector<int> fIndices;
  // To optimize :
  const BatchLab::BaseTuple* fBaseTuple;  
  std::vector<BatchLab::BaseColumn*> fBaseCols;

private:
  class ExpFunc_AIDA_FUNC : public Lib::ExpFunc {
  public:
    virtual bool eval(const Args& a_args,
                      Lib::Value& a_return,
                      std::string& aError) {
      if(a_args.size()!=fNumArgs) return false;
      if(a_args[0].type()==Slash::Core::IValue::ARRAY_DOUBLE) {
      } else {
        std::vector<double> ds(fNumArgs);
        std::vector<Lib::Value>::const_iterator it;
        std::vector<double>::iterator it2 = ds.begin();
        for(it=a_args.begin();it!=a_args.end();++it,++it2) {
          if(!(*it).to_double(*it2)) {
            aError = 
              "Lib::Expression::execute : can't convert Variable to double.";
            a_return.setNone();
            return false;
          }
        }
        a_return.set(fFunction.value(ds));
        return true;
      }
      return false;
    }
    virtual Lib::IExpFunc* copy() const { return new ExpFunc_AIDA_FUNC(*this);}
  public:
    ExpFunc_AIDA_FUNC(const std::string& aName,AIDA::IFunction& aFunction)
    :ExpFunc(aName,aFunction.dimension()),fFunction(aFunction){}
    virtual ~ExpFunc_AIDA_FUNC(){}
  private:
    ExpFunc_AIDA_FUNC(const ExpFunc_AIDA_FUNC& aFrom)
    : Lib::ExpFunc(aFrom),fFunction(aFrom.fFunction){}
    ExpFunc_AIDA_FUNC& operator=(const ExpFunc_AIDA_FUNC& aFrom){
      Lib::ExpFunc::operator=(aFrom);
      return *this;
    }
  private:
    AIDA::IFunction& fFunction;
  };

protected:
  Lib::Processor* fProcessor;
  std::string fScript;
  std::vector<Lib::Value> fVals;
  Lib::Value fResult;
};

}

#endif
