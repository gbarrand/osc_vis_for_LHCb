//
//  Deprecated. In principle Variable is no more used
// in OSC-16.1.
//  But still used in Panoramix !
//

#ifndef Lib_Variable_h
#define Lib_Variable_h

namespace Slash {namespace Core {class IWriter;}}
namespace Slash {namespace Core {class IValue;}}

#include <string>

#include <inlib/array>
#include <inlib/sprintf>
#include <inlib/sto>

#include <Lib/typedefs.h>
////#include <Lib/Debug.h>

namespace Lib {

class Variable {
public: // Internal :
  enum eType {
    NONE = 0,
    INTEGER = 1,
    DOUBLE = 2,
    STRING = 3,
    POINTER = 4,
    UNSIGNED_INTEGER = 5,
    BOOLEAN = 6,
    SHORT = 7,
    FLOAT = 9,
    UNSIGNED_CHAR = 10,
    CHAR = 11,
    VECTOR_DOUBLE = 12,
    INT64 = 14,
    UNSIGNED_INT64 = 15,
    UNSIGNED_SHORT = 16,
    DOUBLE_STAR = 101,
    FLOAT_STAR = 102,
    INT_STAR = 103
  };
  inline static std::string stype(eType);
public:
  Variable(Slash::Core::IWriter& aPrinter)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = NONE;
    u.f_unsigned_int64 = 0;
  }

  virtual ~Variable(){
    delete fName;
    reset();
    //Debug::decrement("Lib::Variable");
  }

public:
  Variable(const Variable& aVariable)
  :fPrinter(aVariable.fPrinter),fName(0),fDimension(aVariable.fDimension){
    //Debug::increment("Lib::Variable");
    if(aVariable.fName) fName = new std::string(*aVariable.fName);
    fType = aVariable.fType;
    if(aVariable.fType==STRING) {
      u.f_std_string = new std::string(*aVariable.u.f_std_string);
    } else if(aVariable.fType==VECTOR_DOUBLE) {
      u.f_Array_double = new inlib::array<double>(*aVariable.u.f_Array_double);
    } else {
      u = aVariable.u;
    }
  }

  Variable& operator=(const Variable& aVariable){
    delete fName;
    fName = 0;
    if(aVariable.fName) fName = new std::string(*aVariable.fName);
    fDimension = aVariable.fDimension;  
    reset();  
    fType = aVariable.fType;
    if(aVariable.fType==STRING) {
      u.f_std_string = new std::string(*aVariable.u.f_std_string);
    } else if(aVariable.fType==VECTOR_DOUBLE) {
      u.f_Array_double = new inlib::array<double>(*aVariable.u.f_Array_double);
    } else {
      u = aVariable.u;
    }  
    return *this;
  }
public:
  // By value :
  Variable(Slash::Core::IWriter& aPrinter,char aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = CHAR;
    u.f_char = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,short aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = SHORT;
    u.f_short = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,int aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = INTEGER;
    u.f_int = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,float aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = FLOAT;
    u.f_float = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,double aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = DOUBLE;
    u.f_double = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,unsigned char aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = UNSIGNED_CHAR;
    u.f_unsigned_char = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,unsigned short aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = UNSIGNED_SHORT;
    u.f_unsigned_short  = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,unsigned int aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = UNSIGNED_INTEGER;
    u.f_unsigned_int  = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,Lib::int64 aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = INT64;
    u.f_int64 = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,Lib::uint64 aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = UNSIGNED_INT64;
    u.f_unsigned_int64 = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,const char* aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = STRING;
    u.f_std_string = new std::string(aValue?aValue:"");
  }
  Variable(Slash::Core::IWriter& aPrinter,const std::string& aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = STRING;
    u.f_std_string = new std::string(aValue);
  }
  Variable(Slash::Core::IWriter& aPrinter,void* aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = POINTER;
    u.f_void_star = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,double* aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = DOUBLE_STAR;
    u.f_double_star = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,float* aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = FLOAT_STAR;
    u.f_float_star = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,int* aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = INT_STAR;
    u.f_int_star = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,bool aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = BOOLEAN;
    u.f_bool = aValue;
  }
  Variable(Slash::Core::IWriter& aPrinter,const inlib::array<double>& aValue)
  :fPrinter(aPrinter),fName(0),fDimension(0){
    //Debug::increment("Lib::Variable");
    fType = VECTOR_DOUBLE;
    u.f_Array_double = new inlib::array<double>(aValue);
  }
public:
  //void print(Slash::Core::IWriter&) const;

  std::string toString() const {
    std::string s;
    switch(fType) {
    case NONE:
      inlib::sprintf(s,5,"(nil)");
      return s;
    case INTEGER:
      inlib::sprintf(s,16,"%d",u.f_int);
      return s;
    case DOUBLE:
      inlib::sprintf(s,16,"%g",u.f_double);
      return s;
    case POINTER:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_void_star);
      return s;
    case UNSIGNED_SHORT:
      inlib::sprintf(s,16,"%u",u.f_unsigned_short);
      return s;
    case UNSIGNED_INTEGER:
      inlib::sprintf(s,16,"%u",u.f_unsigned_int);
      return s;
    case BOOLEAN:
      inlib::sprintf(s,5,"%s",u.f_bool?"true":"false");
      return s;
    case SHORT:
      inlib::sprintf(s,16,"%d",u.f_short);
      return s;
    case INT64:
      inlib::sprintf(s,16,LIB_INT64_FORMAT,u.f_int64);
      return s;
    case UNSIGNED_INT64:
      inlib::sprintf(s,16,LIB_UINT64_FORMAT,u.f_unsigned_int64);
      return s;
    case FLOAT:
      inlib::sprintf(s,16,"%g",u.f_float);
      return s;
    case UNSIGNED_CHAR:
      inlib::sprintf(s,16,"%c",u.f_unsigned_char);
      return s;
    case CHAR:
      inlib::sprintf(s,16,"%c",u.f_char);
      return s;
    case DOUBLE_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_double_star);
      return s;
    case FLOAT_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_float_star);
      return s;
    case INT_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_int_star);
      return s;
    case STRING:
      return *u.f_std_string;
    case VECTOR_DOUBLE:
     {const std::vector<double>& stdv = u.f_Array_double->vector();
      unsigned int number = stdv.size();
      for(unsigned int index=0;index<number;index++) {
        if(index) s+= "\n";
        s += inlib::to<double>(stdv[index]);
      }}
      return s;
    }
  }

  bool to_double(double& aValue){
    switch(fType) {
    case INTEGER:
      aValue = u.f_int;
      return true;
    case DOUBLE:
      aValue = u.f_double;
      return true;
    case UNSIGNED_SHORT:
      aValue = u.f_unsigned_short;
      return true;
    case UNSIGNED_INTEGER:
      aValue = u.f_unsigned_int;
      return true;
    case SHORT:
      aValue = u.f_short;
      return true;
    case INT64:
      aValue = (double)u.f_int64;
      return true;
    case UNSIGNED_INT64:
      aValue = (double)u.f_unsigned_int64;
      return true;
    case FLOAT:
      aValue = u.f_float;
      return true;
    case UNSIGNED_CHAR:
      aValue = u.f_unsigned_char;
      return true;
    case CHAR:
      aValue = u.f_char;
      return true;
    case BOOLEAN:
      aValue = u.f_bool?1:0;
      return true;
    case VECTOR_DOUBLE:
    case NONE:
    case STRING:
    case POINTER:
    case DOUBLE_STAR:
    case FLOAT_STAR:
    case INT_STAR:
      break;
    }
    setNone();
    return false;
  }

  //bool to_bool(bool&)) const;

  void setName(const std::string& aString){
    delete fName;
    fName = new std::string(aString);
  }
  std::string name() const {return fName?*fName:"";}
  void setDimension(unsigned int aDimension){fDimension = aDimension;}
  unsigned int dimension() const {return fDimension;}

  eType type() const {return fType;}

  void setType(eType aType){
    reset();
    fType = aType;
    switch(aType) {
    case NONE:     u.f_unsigned_int64 = 0;break;
    case INTEGER:  u.f_int = 0;break;
    case DOUBLE:   u.f_double = 0;break;
    case POINTER:  u.f_void_star = 0;break;
    case BOOLEAN:  u.f_bool = false;break;
    case SHORT:    u.f_short = 0;break;
    case INT64:    u.f_int64 = 0;break;
    case FLOAT:    u.f_float = 0;break;
    case CHAR:     u.f_char = 0;break;
    case UNSIGNED_CHAR:    u.f_unsigned_char = 0;break;
    case UNSIGNED_SHORT:   u.f_unsigned_short = 0;break;
    case UNSIGNED_INTEGER: u.f_unsigned_int = 0;break;
    case UNSIGNED_INT64:   u.f_unsigned_int64 =0;break;
    case DOUBLE_STAR:      u.f_double_star = 0;break;
    case FLOAT_STAR:       u.f_float_star = 0;break;
    case INT_STAR:         u.f_int_star = 0;break;
    case STRING:           u.f_std_string = new std::string("");break;
    case VECTOR_DOUBLE:    u.f_Array_double = new inlib::array<double>();break;
    }
  }

  void setNone(){
    reset();
    fType = NONE;
    u.f_unsigned_int64 = 0;
  }

  std::string stype() const {return stype(fType);}
  
  bool is_none() const {return (fType==NONE ? true : false);}

  bool is_bool() const {return (fType==BOOLEAN ? true : false);}
  bool is_double() const {return (fType==DOUBLE ? true : false);}
  bool is_vector_double() const {return (fType==VECTOR_DOUBLE ? true : false);}

  unsigned char get_unsigned_char() const {return u.f_unsigned_char;}
  char get_char() const {return u.f_char;}
  unsigned short get_unsigned_short() const {return u.f_unsigned_short;}
  unsigned int get_unsigned_int() const {return u.f_unsigned_int;}
  int get_int() const {return u.f_int;}
  Lib::int64 get_int64() const {return u.f_int64;}
  short get_short() const {return u.f_short;}
  float get_float() const {return u.f_float;}
  double get_double() const {return u.f_double;}
  void* get_void_star() const {return u.f_void_star;}
  double* get_double_star() const {return u.f_double_star;}
  float* get_float_star() const {return u.f_float_star;}
  int* get_int_star() const {return u.f_int_star;}
  bool get_bool() const {return u.f_bool;}
  std::string get_std_string() const {return *u.f_std_string;}
  inlib::array<double>& get_Array_double() {return *u.f_Array_double;}
  
  void set(char aValue){reset();fType = CHAR;u.f_char = aValue;}
  void set(short aValue){reset();fType = SHORT;u.f_short = aValue;}
  void set(int aValue){reset();fType = INTEGER;u.f_int = aValue;}
  void set(float aValue){reset();fType = FLOAT;u.f_float = aValue;}
  void set(double aValue){reset();fType = DOUBLE;u.f_double = aValue;}
  void set(unsigned char aValue){
    reset();
    fType = UNSIGNED_CHAR;
    u.f_unsigned_char = aValue;
  }
  void set(unsigned short aValue){
    reset();
    fType = UNSIGNED_SHORT;
    u.f_unsigned_short  = aValue;
  }
  void set(unsigned int aValue){
    reset();
    fType = UNSIGNED_INTEGER;
    u.f_unsigned_int  = aValue;
  }
  void set(Lib::int64 aValue){reset();fType = INT64;u.f_int64 = aValue;}
  void set(Lib::uint64 aValue){
    reset();
    fType = UNSIGNED_INT64;
    u.f_unsigned_int64 = aValue;
  }
  void set(const std::string& aValue) {
    reset();
    fType = STRING;
    u.f_std_string = new std::string(aValue);
  }
  void set(const char* aValue){
    reset();
    fType = STRING;
    u.f_std_string = new std::string(aValue);
  }

  void set(const inlib::array<double>& aValue){
    reset();
    fType = VECTOR_DOUBLE;
    u.f_Array_double = new inlib::array<double>(aValue);
  }
  void set(void* aValue){reset();fType = POINTER;u.f_void_star = aValue;}
  void set(double* aValue){
    reset();
    fType = DOUBLE_STAR;
    u.f_double_star = aValue;
  }
  void set(float* aValue){reset();fType = FLOAT_STAR;u.f_float_star = aValue;}
  void set(int* aValue){reset();fType = INT_STAR;u.f_int_star = aValue;}
  void set(bool aValue){reset();fType = BOOLEAN;u.f_bool = aValue;}

  inline bool set(const Slash::Core::IValue&);

  // Used in Columns.y :
  inline bool assign(const Variable&,std::string&);

  // operations in Expression.cxx :
  inline bool minus(std::string&);
  inline bool do_not(std::string&);

  inline bool add(const Variable&,std::string&);
  inline bool subtract(const Variable&,std::string&);
  inline bool multiply(const Variable&,std::string&);
  inline bool divide(const Variable&,std::string&);
    
  inline bool if_gt(const Variable&,std::string&);
  inline bool if_eq(const Variable&,std::string&);
  inline bool if_ne(const Variable&,std::string&);
  inline bool if_ge(const Variable&,std::string&);
  inline bool if_lt(const Variable&,std::string&);
  inline bool if_le(const Variable&,std::string&);
  inline bool if_and(const Variable&,std::string&);
  inline bool if_or(const Variable&,std::string&);

  bool cxx_type(std::string& aValue) const {
    switch(fType) {
    case INTEGER: 
      aValue = "int";
      return true;
    case DOUBLE: 
      aValue = "double";
      return true;
    case STRING: 
      aValue = "std::string";
      return true;
    case POINTER: 
      aValue = "void*";
      return true;
    case BOOLEAN: 
      aValue = "bool";
      return true;
    case SHORT: 
      aValue = "short";
      return true;
    case FLOAT: 
      aValue = "float";
      return true;
    case CHAR: 
      aValue = "char";
      return true;
    case UNSIGNED_CHAR: 
      aValue = "unsigned char";
      return true;
    case UNSIGNED_SHORT: 
      aValue = "unsigned short";
      return true;
    case UNSIGNED_INTEGER: 
      aValue = "unsigned int";
      return true;
    case DOUBLE_STAR: 
      aValue = "double*";
      return true;
    case FLOAT_STAR: 
      aValue = "float*";
      return true;
    case INT_STAR: 
      aValue = "int*";
      return true;
    case VECTOR_DOUBLE:
    case NONE:
    case INT64:
    case UNSIGNED_INT64:
      aValue = "";
      return false;
    }
    return false;
  }
public:
  inline static Slash::Core::IValue* toValue(const Lib::Variable&);
private:
  void reset(){
    if(fType==STRING) {
      delete u.f_std_string;
      u.f_std_string = 0;
    } else if(fType==VECTOR_DOUBLE) {
      delete u.f_Array_double;
      u.f_Array_double = 0;
    } else {
      u.f_unsigned_int64 = 0;
    }
  }
private:
  Slash::Core::IWriter& fPrinter;
  std::string* fName;
  unsigned int fDimension;
private:
  eType fType;
  union {
    bool f_bool;
    char f_char;
    int f_int;
    short f_short;
    Lib::int64 f_int64;
    float f_float;
    double f_double;
    unsigned char f_unsigned_char;
    unsigned int f_unsigned_short;
    unsigned int f_unsigned_int;
    Lib::uint64 f_unsigned_int64;
    void* f_void_star;
    double* f_double_star;
    float* f_float_star;
    int* f_int_star;
    std::string* f_std_string;
    inlib::array<double>* f_Array_double;
  } u;
};

}

#define Lib_Variable_error_div_zero "Lib::Variable::divide : divide by zero."

#endif

#include "Variable.icc"

