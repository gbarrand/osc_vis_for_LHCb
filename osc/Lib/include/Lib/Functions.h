#ifndef Lib_Functions_h
#define Lib_Functions_h

// Inheritance :
#include <Slash/Data/IFunction.h>

#include <vector>
#include <string>
#include <ostream>

#include <inlib/sort>
#include <inlib/strip>
#include <inlib/touplow>

#include <Lib/Processor.h>

namespace Lib {

namespace Function {

class Scripted {
public:
  Scripted(const std::string& aString,std::ostream& a_out)
  :fScript(aString),fIsValid(false),fDimension(0),
  f_out(a_out),fProcessor("",a_out)
  {
    set(aString);
  }
  virtual ~Scripted(){}
public:
  Scripted(const Scripted& aFrom)
  :fScript(aFrom.fScript),fIsValid(false)
  ,fDimension(aFrom.fDimension),fParameterNames(aFrom.fParameterNames)
  ,fParameterValues(aFrom.fParameterValues)
  ,fVariableNames(aFrom.fVariableNames)
  ,f_out(aFrom.f_out),fProcessor("",aFrom.f_out)
  {
    set(aFrom.fScript);
  }

  Scripted& operator=(const Scripted& aFrom){
    fScript = aFrom.fScript;
    fDimension = aFrom.fDimension;
    fParameterNames = aFrom.fParameterNames;
    fParameterValues = aFrom.fParameterValues;
    fVariableNames = aFrom.fVariableNames;
  
    //f_out = aFrom.f_out;
    fIsValid = false;
  
    set(aFrom.fScript);
    return *this;
  }

public:
  bool isValid() const{return fIsValid;}
  unsigned int dimension() const {return fDimension;}
  std::string script() const {return fScript;}
  std::vector<std::string> parameterNames() const {return fParameterNames;}
  std::vector<double> parameterValues() const {return fParameterValues;}
  std::vector<std::string> variableNames() const {return fVariableNames;}

  bool set(const std::string& aString){
    reset();
    //Check for AIDA/SLAC variable syntax convention x[<int>] :
    std::string ss = aString;
    std::string ns;
    bool slac_syntax = false;
    while(true) {
      std::string::size_type lb = ss.find("x[");
      if(lb==std::string::npos){
        ns += ss;
        break;
      }
      std::string::size_type rb = ss.find(']',lb+2);
      if(rb==std::string::npos) {
        f_out << "Lib::Function::Scripted::set :"
            << " bad [] balance in " << inlib::sout(aString)
            << std::endl;
        reset();
        return false;
      }
      //     abx[012]ef
      //     0 2    7 9   size=10
      std::string snum = ss.substr(lb+2,rb-lb-2);
      int coord;
      if(!inlib::to<int>(snum,coord)) {
        f_out << "Lib::Function::Scripted::set :"
            << " integer expected within [] in " << inlib::sout(aString)
            << ". Found " << inlib::sout(snum) << "."
            << std::endl;
        reset();
        return false;
      }
      slac_syntax = true;
      ns += ss.substr(0,lb)+ "x"+ snum;
      ss = ss.substr(rb+1,ss.size()-rb);
    }
    //printf("debug : ns \"%s\"\n",ns.c_str());
  
    // Find variables in the script :
    fProcessor.setScript(ns);
    std::vector<std::string> vars;
    if(!fProcessor.findVariables(vars)) {
      f_out << "Lib::Function::Scripted::set :"
            << " Lib::Processor can't find variables in "
            << inlib::sout(aString)
            << std::endl;
      reset();
      return false;
    }
  
    //vars had been passed to inlib::unique.
  
    //  In vars, some are expected to be of the form x<int>.
    // The x<int> are the variables of the scripted function. 
    // The rest are the parameters of the scripted function. 
    // The variables are used to find the dimension of the function.
    //  If x0 only is found, then it is a one dim function.
    // If x0,x1 only are found, then it is a two dim function, etc...
  
    std::vector<int> coords;
    bool found_x = false;
    bool found_y = false;
    bool found_z = false;
   {for(unsigned int index=0;index<vars.size();index++) {
      int coord;
      if(vars[index]=="x") {
        found_x = true;
      } else if(vars[index]=="y") {
        found_y = true;
      } else if(vars[index]=="z") {
        found_z = true;
      } else if(isVariable(vars[index],coord)) {
        coords.push_back(coord);
      } else {
        fParameterNames.push_back(vars[index]);
      }
    }}
  
  
    vars.clear(); //Reorder with params first and variables last.
   {for(unsigned int index=0;index<fParameterNames.size();index++) {
      vars.push_back(fParameterNames[index]);
    }}
  
    // Sort coords :
    if(coords.size()) {
      if(found_x||found_y||found_z) {      
        f_out << "Lib::Function::Scripted::set :"
            << " inconsistent variable specification in "
            << inlib::sout(aString)
            << std::endl;
        reset();
        return false;
      }    
      inlib::sort::sort<int>(coords);
      for(unsigned int index=0;index<coords.size();index++) {
        std::string s;
        inlib::sprintf(s,32,"x%d",coords[index]);
        vars.push_back(s);
        if(slac_syntax) {
          inlib::sprintf(s,32,"x[%d]",coords[index]);
          fVariableNames.push_back(s);
        } else {
          fVariableNames.push_back(s);
        }
      }
    } else {
      if(found_x) {
        vars.push_back("x");
        fVariableNames.push_back("x");
      }
      if(found_y) {
        vars.push_back("y");
        fVariableNames.push_back("y");
      }
      if(found_z) {
        vars.push_back("z");
        fVariableNames.push_back("z");
      }
    }
  
    if(!fProcessor.compile(vars)) {
      f_out << "Lib::Function::Scripted::set :"
          << " Lib::Processor can't compile " << inlib::sout(aString)
          << std::endl;
      reset();
      return false;
    }
  
    fParameterValues.resize(fParameterNames.size(),0);
  
    fInput.resize
      (fParameterNames.size()+fVariableNames.size(),Lib::Value((double)0));
  
    fScript = aString;
    fDimension = fVariableNames.size();
    fIsValid = true;
    return true;
  }

  bool setParameters(const std::vector<double>& aParams){
    if(aParams.size()!=fParameterValues.size()) return false;
    fParameterValues = aParams;
    return true;
  }

  bool value(const std::vector<double>& aXs,double& aValue,
             std::string& aError) const {
    INLIB_SELF(Scripted);

    if(!isValid()) {
      aValue = 0;
      aError = "Lib::Function::Scripted::value : is not valid.";
      return false;
    }
    if(aXs.size()!=fDimension) {
      aValue = 0;
      aError = 
        "Lib::Function::Scripted::value : xs.size()/dimension mismatch.";
      return false;
    }

    unsigned int ii = 0;
   {unsigned int number = fParameterValues.size(); 
    for(unsigned int index=0;index<number;index++) {
      self.fInput[ii].set(fParameterValues[index]);
      ii++;
    }}
   {unsigned int number = aXs.size(); 
    for(unsigned int index=0;index<number;index++) {
      self.fInput[ii].set(aXs[index]);
      ii++;
    }}
  
    Lib::Value var;
    if(!self.fProcessor.execute(fInput,var,aError)) {
      aValue = 0;
      return false;
    }
  
    if(var.type()!=Slash::Core::IValue::DOUBLE) {
      aError = std::string("Lib::Function::Scripted::value :")
             + " result is not a double Variable.";
      aValue = 0;
      return false;
    }
  
    //aValue = var.toDouble(); //FIXME
    aValue = var.get_double();
    return true;
  }

private:
  static bool isVariable(const std::string& aVar,int& aIndex){
    // If aVar is not of the form x<int> return false.
    // If yes, return true with aIndex=<int>.
    if(aVar.size()<=1) {
      aIndex = 0;
      return false;
    }
    if(aVar[0]!='x') {
      aIndex = 0;
      return false;
    }
    if(!inlib::to<int>(aVar.substr(1,aVar.size()-1),aIndex)) {
      aIndex = 0;
      return false;;
    }
    return true;
  }

  void reset(){
    // Reset :
    fIsValid = false;
    fScript = "";
    fDimension = 0;
    fParameterNames.clear();
    fParameterValues.clear();
    fVariableNames.clear();
    fProcessor.clear();
    fInput.clear();
  }

private:
  std::string fScript;
  bool fIsValid;
  unsigned int fDimension;
  std::vector<std::string> fParameterNames;
  std::vector<double> fParameterValues;
  std::vector<std::string> fVariableNames;
  std::ostream& f_out;
  Processor fProcessor;
  std::vector<Lib::Value> fInput;
};

class Composite {
public:
  Composite(const std::string& aString,std::ostream& a_out)
  :fScript(aString),fScripted("0",a_out),f_out(a_out){
    set(aString);
  }
  virtual ~Composite(){}
public:
  Composite(const Composite& aFrom)
  :fScript(aFrom.fScript),fScripted(aFrom.fScripted),f_out(aFrom.f_out){
    set(aFrom.fScript);
  }
  Composite& operator=(const Composite& aFrom){
    fScript = aFrom.fScript;
    fScripted = aFrom.fScripted;
    //f_out = aFrom.f_out;
    set(aFrom.fScript);
    return *this;
  }
public:
  bool isValid() const {return fScripted.isValid();}
  unsigned int dimension() const {return fScripted.dimension();}
  std::string script() const {return fScript;}
  std::vector<std::string> parameterNames() const {
    return fScripted.parameterNames();
  }
  std::vector<double> parameterValues() const {
    return fScripted.parameterValues();
  }
  std::vector<std::string> variableNames() const {
    return fScripted.variableNames();
  }
  bool setParameters(const std::vector<double>& aParams){
    return fScripted.setParameters(aParams);
  }


  bool value(const std::vector<double>& aXs,double& aValue,
             std::string& aError) const {
    return fScripted.value(aXs,aValue,aError);
  }
  bool set(const std::string& aString){
    // exa : G+E
    // FIXME : have more operations.
    std::string s;
    std::string script; 
    std::vector<std::string> words;
    inlib::words(aString,"+",false,words);
    unsigned int wordn = words.size();
    for(unsigned int index=0;index<wordn;index++) {
      if(index) script += "+";
      std::string& word = words[index];
      inlib::strip(word);
      inlib::touppercase(word);
      std::string p; //variable prefix.
      const char* pc = p.c_str();
      if(word=="E") {
        if(wordn>=2) inlib::sprintf(p,32,"E_%d_",index);
        inlib::sprintf(s,132,"%samplitude*exp(x*%sexponent)",pc,pc);
      } else if(word=="EHBOOK") {
        if(wordn>=2) inlib::sprintf(p,32,"EHBOOK_%d_",index);
        inlib::sprintf(s,132,"expo(x,%sA,%sB)",pc,pc);
      } else if(word=="G") {
        if(wordn>=2) inlib::sprintf(p,32,"G_%d_",index);
        inlib::sprintf
          (s,132,"gauss(x,%samplitude,%smean,%ssigma)",pc,pc,pc);
      } else if(word=="BW") {
        if(wordn>=2) inlib::sprintf(p,32,"BW_%d_",index);
        inlib::sprintf
          (s,132,"breit(x,%samplitude,%sorigin,%swidth)",pc,pc,pc);
      } else if(word=="P1") {
        if(wordn>=2) inlib::sprintf(p,32,"P1_%d_",index);
        inlib::sprintf(s,132,"pol1(x,%sp0,%sp1)",pc,pc);
      } else if(word=="P2") {
        if(wordn>=2) inlib::sprintf(p,32,"P2_%d_",index);
        inlib::sprintf(s,132,"pol2(x,%sp0,%sp1,%sp2)",pc,pc,pc);
      } else if(word=="P3") {
        if(wordn>=2) inlib::sprintf(p,32,"P3_%d_",index);
        inlib::sprintf
          (s,132,"pol3(x,%sp0,%sp1,%sp2,%sp3)",pc,pc,pc,pc);
      } else if(word=="P4") {
        if(wordn>=2) inlib::sprintf(p,32,"P4_%d_",index);
        inlib::sprintf
          (s,132,"pol4(x,%sp0,%sp1,%sp2,%sp3,%sp4)",pc,pc,pc,pc,pc);
      } else {
        f_out << "Lib::Function::Composite::set :" 
            << " in " << inlib::sout(aString)
            << ", unknown function " << inlib::sout(word)
            << std::endl;
        fScript = "";
        return false;
      }
      script += s;
    }
  
    //printf("debug : composite : \"%s\"\n",script.c_str());
  
    if(!fScripted.set(script)) {
      fScript = "";
      return false;
    }
    fScript = aString;
    return true;
  }

private:
  static bool isVariable(const std::string&,int&);
  void reset();
private:
  std::string fScript;
  Scripted fScripted;
  std::ostream& f_out;
};

class Compiled : public virtual Slash::Data::IFunction {
public: // IFunction :
  virtual std::string file() const {return fFile;}
  virtual std::string model() const {return fModel;}
  virtual void* address() const {return fAddress;}

  virtual unsigned int dimension() const {
    unsigned int dim = 0;
    unsigned int dimn = fDims.size();
    for(unsigned int i=0;i<dimn;i++) dim += (fDims[i]?fDims[i]:1);
    return dim;
  }

  virtual unsigned int numberOfParameters() const {return fParn;}

  virtual bool value(const std::vector<double>& aArgs,double& aValue) const {
    if(!fAddress) {
      aValue = 0;
      return false;
    }
  
    if(aArgs.size()!=dimension()) {
      aValue = 0;
      return false;
    }
  
    bool status = true;
  
    unsigned int argn = fValues.size();
    unsigned int aArgn = aArgs.size();
  
   {unsigned int argc = 0;
    for(unsigned int argi=0;argi<argn;argi++) {
      Slash::Core::IValue* ivalue = fValues[argi];
  
      if(ivalue->type()==Slash::Core::IValue::DOUBLE) {
        if((argc+1)>aArgn) {
          status = false;
          break;
        } else {
          ivalue->set(aArgs[argc]);
          argc++;
        }
      } else if(ivalue->type()==Slash::Core::IValue::FLOAT) {
        if((argc+1)>aArgn) {
          status = false;
          break;
        } else {
          ivalue->set((float)aArgs[argc]);
          argc++;
        }
      } else if(ivalue->type()==Slash::Core::IValue::INT) {
        if((argc+1)>aArgn) {
          status = false;
          break;
        } else {
          ivalue->set((int)aArgs[argc]);
          argc++;
        }
      } else if(ivalue->type()==Slash::Core::IValue::DOUBLE_STAR) {
        unsigned int dim = fDims[argi];
        if((argc+dim)>aArgn) {
          status = false;
          break;
        } else {
          double* array = ivalue->get_double_star();
          for(unsigned int i=0;i<dim;i++) {
            array[i] = aArgs[argc];
            argc++;
          }
        }
  
      } else if(ivalue->type()==Slash::Core::IValue::INT_STAR) {
        unsigned int dim = fDims[argi];
        if((argc+dim)>aArgn) {
          status = false;
          break;
        } else {
          int* array = ivalue->get_int_star();
          for(unsigned int i=0;i<dim;i++) {
            array[i] = (int)aArgs[argc];
            argc++;
          }
        }
  
      } else if(ivalue->type()==Slash::Core::IValue::FLOAT_STAR) {
        unsigned int dim = fDims[argi];
        if((argc+dim)>aArgn) {
          status = false;
          break;
        } else {
          float* array = ivalue->get_float_star();
          for(unsigned int i=0;i<dim;i++) {
            array[i] = (float)aArgs[argc];
            argc++;
          }
        }
  
      } else {
        status = false;
        break;
      }
    }
    if(argc!=dimension()) {
      status = false;
    }}
    //if(!status) {
      //printf("debug : Lib::Function::Compiled::value : arg setup failed.\n");
    //}
  
    if(status) {
      typedef bool (*Function)(const std::vector<Slash::Core::IValue*>&,
                               Slash::Core::IValue&);
      status = ((Function)fAddress)(fValues,*fReturn);
    }
   
    if(status) {
      status = true;
      if(fReturn->type()==Slash::Core::IValue::FLOAT) {
        aValue = fReturn->get_float();
      } else if(fReturn->type()==Slash::Core::IValue::DOUBLE) {
        aValue = fReturn->get_double();
      } else {
        status = false;    
      }
    }
  
    return status;  
  }
  
  virtual bool value(const std::vector<Slash::Core::IValue*>& aArgs,
                     Slash::Core::IValue& aValue) const {
    if(!fAddress) {
      aValue.setNone();
      return false;
    }
  
    typedef bool (*Function)(const std::vector<Slash::Core::IValue*>&,
                             Slash::Core::IValue&);
  
    return ((Function)fAddress)(aArgs,aValue);
  }

  virtual std::vector<Slash::Core::IValue*> signature(Slash::Core::IValue*& aReturn) const {
    // args + return value.
    aReturn = fReturn;
    return fValues;
  }

public:
  Compiled(std::ostream& a_out,
           const std::string& aFile,const std::string& aModel,
           void* aAddress,const std::vector<Lib::Value>& aArgs,
           unsigned int aParn)
  :f_out(a_out)
  ,fFile(aFile)
  ,fModel(aModel)
  ,fAddress(aAddress)
  ,fParn(aParn)
  ,fReturn(0)
  {
    Lib::Debug::increment("Lib::Function::Compiled");
  
    unsigned int argn = aArgs.size();
    for(unsigned int argi=0;argi<argn;argi++) {
      const Lib::Value& v = aArgs[argi];
      if( (v.type()==Slash::Core::IValue::DOUBLE_STAR) ||
          (v.type()==Slash::Core::IValue::INT_STAR)    ||
          (v.type()==Slash::Core::IValue::FLOAT_STAR)  ||
          (v.type()==Slash::Core::IValue::DOUBLE)      ||
          (v.type()==Slash::Core::IValue::INT)         ||
          (v.type()==Slash::Core::IValue::FLOAT)       ){
        //ok
      } else {
        //unknown type :
        f_out << "Lib::Function::Compiled::Compiled :"
            << " " << inlib::sout(v.stype()) << " not handled."
            << std::endl;
        fAddress = 0;
        fDims.clear();
        return;
      }
      fDims.push_back(v.dimension());
      //printf("debug :  arg \"%s\" %d\n",NAME.c_str(),dim);
    }
  
    // to optimize :
   {fValues.resize(argn,0);
    for(unsigned int argi=0;argi<argn;argi++) {
      const Lib::Value& v = aArgs[argi];
      if(v.type()==Slash::Core::IValue::DOUBLE) {
        fValues[argi] = new Lib::Value(double(0));
      } else if(v.type()==Slash::Core::IValue::FLOAT) {
        fValues[argi] = new Lib::Value(float(0));
      } else if(v.type()==Slash::Core::IValue::INT) {
        fValues[argi] = new Lib::Value(int(0));
      } else if(v.type()==Slash::Core::IValue::DOUBLE_STAR) {
        unsigned int dim = fDims[argi];
        double* array = dim ? new double[dim] : 0;
        fValues[argi] = new Lib::Value(array);
      } else if(v.type()==Slash::Core::IValue::INT_STAR) {
        unsigned int dim = fDims[argi];
        int* array = dim ? new int[dim] : 0;
        fValues[argi] = new Lib::Value(array);
      } else if(v.type()==Slash::Core::IValue::FLOAT_STAR) {
        unsigned int dim = fDims[argi];
        float* array = dim? new float[dim] : 0;
        fValues[argi] = new Lib::Value(array);
      }
    }}
    fReturn = new Lib::Value;  
  }

  virtual ~Compiled(){
    delete fReturn;
   {unsigned int argn = fValues.size();
    for(unsigned int argi=0;argi<argn;argi++) {
      Slash::Core::IValue* ivalue = fValues[argi];
      if(ivalue->type()==Slash::Core::IValue::FLOAT_STAR) {
        if(ivalue->get_float_star()) delete [] ivalue->get_float_star();
      } else if(ivalue->type()==Slash::Core::IValue::DOUBLE_STAR) {
        if(ivalue->get_double_star()) delete [] ivalue->get_double_star();
      } else if(ivalue->type()==Slash::Core::IValue::INT_STAR) {
        if(ivalue->get_int_star()) delete [] ivalue->get_int_star();
      }
      delete ivalue;
    }}
    Lib::Debug::decrement("Lib::Function::Compiled");
  }
private:
  std::ostream& f_out;
  std::string fFile;
  std::string fModel;
  void* fAddress;
  std::vector<unsigned int> fDims;
  unsigned int fParn;
  std::vector<Slash::Core::IValue*> fValues;
  Slash::Core::IValue* fReturn;
};

} //Function

} //Lib

#endif




