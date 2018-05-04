#ifndef Lib_ExpFunc_h
#define Lib_ExpFunc_h

// expression function.

#include <Lib/IExpFunc.h>

namespace Lib {namespace Function {class InsidePolygon;}}

namespace Lib {

class ExpFunc : public virtual IExpFunc {
public: //IExpFunc
public:
  virtual unsigned int numberOfArguments() const { return fNumArgs;}
  virtual std::string name() const { return fName;}
public:
  ExpFunc(const std::string& aName,unsigned int aNumber)
  :fName(aName),fNumArgs(aNumber){
    Debug::increment("Lib::ExpFunc");
  }
  virtual ~ExpFunc(){ 
    Debug::decrement("Lib::ExpFunc");
  }
protected:
  ExpFunc(const ExpFunc& aFrom) 
  :IExpFunc(),fName(aFrom.fName),fNumArgs(aFrom.fNumArgs){
    Debug::increment("Lib::ExpFunc");
  }
  ExpFunc& operator=(const ExpFunc& aFrom) {
    fName = aFrom.fName;
    fNumArgs = aFrom.fNumArgs;
    return *this;
  }
protected:
  typedef double(*Func0)(double);
  typedef bool(*InDomain)(double);
  typedef double(*Func1)(double,double);

  bool eval0(const Lib::Value& a_x,
             Lib::Value& a_return,std::string& aError,
             Func0 aFunc) {

    if(a_x.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
  
     {std::vector<unsigned int> is;
      const std::vector<double>& stdv = a_x.get_array_double(is);
      a_return.set(is,stdv);}
  
      // In the below, we need a non const method.
      std::vector<double>& stdv = a_return.array_double();
      unsigned int number = stdv.size();
      for(unsigned int index=0;index<number;index++) { 
        stdv[index] = aFunc(stdv[index]);
      }
      return true;
    } else {
      double x;
      if(!TO_D(a_x,x,a_return,aError)) return false;

      a_return.set(aFunc(x));
      return true;
    }
  }
  bool eval0(const Lib::Value& a_x,
             Lib::Value& a_return,std::string& aError,
             Func0 aFunc,InDomain aDomain) {

    if(a_x.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
  
     {std::vector<unsigned int> is;
      const std::vector<double>& stdv = a_x.get_array_double(is);
      a_return.set(is,stdv);}
  
      // In the below, we need a non const method.
      std::vector<double>& stdv = a_return.array_double();
      unsigned int number = stdv.size();
      for(unsigned int index=0;index<number;index++) { 
        double x = stdv[index];
        if(!aDomain(x)) {
          aError = std::string("Expression::execute : ")
  	  + " " + fName + " : variable value "+ inlib::to<double>(x)
                 + " out of domain.";
          a_return.setNone();
          return false;
        }
        stdv[index] = aFunc(x);
      }
      return true;
    } else {
      double x;
      if(!TO_D(a_x,x,a_return,aError)) return false;
  
      if(!aDomain(x)) {
        aError = std::string("Expression::execute : ")
  	+ " " + fName + " : variable value "+ inlib::to<double>(x)
               + " out of domain.";
        a_return.setNone();
        return false;
      }
  
      a_return.set(aFunc(x));
      return true;
    }
  }
  bool eval1(const Lib::Value& a_x,Lib::Value& a_return,std::string& aError,
             const Lib::Value& a_1,Func1 aFunc) {

    double d_1;
    if(!TO_D(a_1,d_1,a_return,aError)) return false;
  
    if(a_x.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
  
     {std::vector<unsigned int> is;
      const std::vector<double>& stdv = a_x.get_array_double(is);
      a_return.set(is,stdv);}
  
      // In the below, we need a non const method.
      std::vector<double>& stdv = a_return.array_double();
      unsigned int number = stdv.size();
      for(unsigned int index=0;index<number;index++) { 
        stdv[index] = aFunc(stdv[index],d_1);
      }
      return true;
    } else {
      double x;
      if(!TO_D(a_x,x,a_return,aError)) return false;
      a_return.set(aFunc(x,d_1));
      return true;
    }
  }

protected:  
  inline static bool TO_D(const Lib::Value& a_vx,double& a_x,
            Lib::Value& a_return,std::string& aError) {
    if(!Lib::Value::to_double(a_vx,a_x)) {
      aError = "Lib::Expression::execute : can't convert Variable to double.";
      a_return.setNone();
      return false;
    }
    return true;
  }
protected:
  std::string fName;
  unsigned int fNumArgs;
};

class ExpFunc_cfunc : public Lib::ExpFunc {
public:
  typedef double(*cfunc)(double);
  typedef bool(*domain_func)(double);
public:
  virtual bool eval(const Args& a_args,Lib::Value& a_return,
                    std::string& aError) {
    if(a_args.size()!=fNumArgs) return false;
    return ExpFunc::eval0(a_args[0],a_return,aError,f_cfunc,f_domain);
  }
  virtual Lib::IExpFunc* copy() const { return new ExpFunc_cfunc(*this);}
public:
  ExpFunc_cfunc(const std::string& aName,cfunc aFunction,domain_func aDomain)
  :ExpFunc(aName,1),f_cfunc(aFunction),f_domain(aDomain){}
  virtual ~ExpFunc_cfunc(){}
private:
  ExpFunc_cfunc(const ExpFunc_cfunc& aFrom)
  : Lib::ExpFunc(aFrom),f_cfunc(aFrom.f_cfunc),f_domain(aFrom.f_domain){}
  ExpFunc_cfunc& operator=(const ExpFunc_cfunc& aFrom){ 
    Lib::ExpFunc::operator=(aFrom);return *this;
  }
private:
  cfunc f_cfunc;
  domain_func f_domain;
};

class ExpFuncs {
public:
  inline ExpFuncs();
  virtual ~ExpFuncs(){ 
    std::vector<IExpFunc*>::iterator it;
    while(!fFuncs.empty()) {
      it = fFuncs.end();
      it--;
      IExpFunc* func = *it;
      fFuncs.erase(it);
      delete func;
    }
    Debug::decrement("Lib::ExpFuncs");
  }
public:
  ExpFuncs(const ExpFuncs& aFrom) {
    for(std::vector<IExpFunc*>::const_iterator it = aFrom.fFuncs.begin();
        it!=aFrom.fFuncs.end();++it) {
      fFuncs.push_back((*it)->copy());
    }  
    Debug::increment("Lib::ExpFuncs");
  }
  ExpFuncs& operator=(const ExpFuncs& aFrom) {
    for(std::vector<IExpFunc*>::const_iterator it = aFrom.fFuncs.begin();
        it!=aFrom.fFuncs.end();++it) {
      fFuncs.push_back((*it)->copy());
    }  
    return *this;
  }
public:
  void add(Lib::IExpFunc* aFunc) {fFuncs.push_back(aFunc);}
  IExpFunc* find(const std::string& aString) const {  
    for(std::vector<IExpFunc*>::const_iterator it = fFuncs.begin();
        it!=fFuncs.end();++it) {
      if((*it)->name()==aString) return (*it);
    }  
    return 0;
  }
private:
  std::vector<IExpFunc*> fFuncs;
};

}

#endif

#include "ExpFunc.icc"


