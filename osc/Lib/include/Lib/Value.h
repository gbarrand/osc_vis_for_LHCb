#ifndef Lib_Value_h
#define Lib_Value_h

// Inheritance :
#include <Slash/Tools/Value.h>

#include <Lib/Debug.h>

namespace Lib {

class Value : public Slash::Value {
public:
  Value(){Debug::increment("Lib::Value");}
 
#define LIB_VALUE_CSTOR(a__type)\
  Value(a__type aValue): Slash::Value(aValue){Debug::increment("Lib::Value");}

  LIB_VALUE_CSTOR(bool)
  LIB_VALUE_CSTOR(char)
  LIB_VALUE_CSTOR(short)
  LIB_VALUE_CSTOR(int)
  LIB_VALUE_CSTOR(unsigned char)
  LIB_VALUE_CSTOR(unsigned short)
  LIB_VALUE_CSTOR(unsigned int)
  LIB_VALUE_CSTOR(Slash::int64)
  LIB_VALUE_CSTOR(Slash::uint64)
  LIB_VALUE_CSTOR(float)
  LIB_VALUE_CSTOR(double)
  LIB_VALUE_CSTOR(void*)
  LIB_VALUE_CSTOR(double*)
  LIB_VALUE_CSTOR(float*)
  LIB_VALUE_CSTOR(int*)
  LIB_VALUE_CSTOR(const char*)
  LIB_VALUE_CSTOR(const std::string&)
  LIB_VALUE_CSTOR(const std::vector<double>&)

#define LIB_VALUE_CSTORA(a__type)\
  Value(const std::vector<unsigned int>& a_is,\
        const std::vector<a__type>& a_value): Slash::Value(a_is,a_value){\
    Debug::increment("Lib::Value");\
  }

  LIB_VALUE_CSTORA(unsigned char)
  LIB_VALUE_CSTORA(char)
  LIB_VALUE_CSTORA(unsigned short)
  LIB_VALUE_CSTORA(short)
  LIB_VALUE_CSTORA(unsigned int)
  LIB_VALUE_CSTORA(int)
  LIB_VALUE_CSTORA(Slash::uint64)
  LIB_VALUE_CSTORA(Slash::int64)
  LIB_VALUE_CSTORA(float)
  LIB_VALUE_CSTORA(double)
  LIB_VALUE_CSTORA(bool)
  LIB_VALUE_CSTORA(std::string)
    
#undef LIB_VALUE_CSTOR
#undef LIB_VALUE_CSTORA

public:
  Value(const Value& aFrom): Slash::Value(aFrom){
    Debug::increment("Lib::Value");
  }

  Value& operator=(const Value& aFrom){
    Slash::Value::operator=(aFrom);
    return *this;
  }

  virtual ~Value(){
    Debug::decrement("Lib::Value");
  }

public:
  inline bool to_double(double& aValue) const {
    //used in BatchLab, OnXLab.
    switch(fType) {
    case Slash::Core::IValue::INT:
      aValue = u.f_int;
      return true;
    case Slash::Core::IValue::DOUBLE:
      aValue = u.f_double;
      return true;
    case Slash::Core::IValue::UNSIGNED_SHORT:
      aValue = u.f_unsigned_short;
      return true;
    case Slash::Core::IValue::UNSIGNED_INT:
      aValue = u.f_unsigned_int;
      return true;
    case Slash::Core::IValue::SHORT:
      aValue = u.f_short;
      return true;
    case Slash::Core::IValue::INT64:
      aValue = (double)u.f_int64;
      return true;
    case Slash::Core::IValue::UNSIGNED_INT64:
      aValue = (double)u.f_unsigned_int64;
      return true;
    case Slash::Core::IValue::FLOAT:
      aValue = u.f_float;
      return true;
    case Slash::Core::IValue::UNSIGNED_CHAR:
      aValue = u.f_unsigned_char;
      return true;
    case Slash::Core::IValue::CHAR:
      aValue = u.f_char;
      return true;
    case Slash::Core::IValue::BOOL:
      aValue = u.f_bool?1:0;
      return true;
    case Slash::Core::IValue::NONE:
    case Slash::Core::IValue::STRING:
    case Slash::Core::IValue::VOID_STAR:
    case Slash::Core::IValue::DOUBLE_STAR:
    case Slash::Core::IValue::FLOAT_STAR:
    case Slash::Core::IValue::INT_STAR:
    case Slash::Core::IValue::ARRAY_UNSIGNED_CHAR:
    case Slash::Core::IValue::ARRAY_CHAR:
    case Slash::Core::IValue::ARRAY_UNSIGNED_SHORT:
    case Slash::Core::IValue::ARRAY_SHORT:
    case Slash::Core::IValue::ARRAY_UNSIGNED_INT:
    case Slash::Core::IValue::ARRAY_INT:
    case Slash::Core::IValue::ARRAY_UNSIGNED_INT64:
    case Slash::Core::IValue::ARRAY_INT64:
    case Slash::Core::IValue::ARRAY_FLOAT:
    case Slash::Core::IValue::ARRAY_DOUBLE:
    case Slash::Core::IValue::ARRAY_BOOL:
    case Slash::Core::IValue::ARRAY_STRING:
      break;
    }
    aValue = 0;
    return false;
  }
  
  inline static std::string error_div_zero() {return "divide by zero.";}

public:
  inline static bool assign(Value&,const Value&,std::string&);

  // operations in Expression.cxx :
  inline static bool minus(Value&,std::string&);
  inline static bool do_not(Value&,std::string&);

  inline static bool add(Value&,const Value&,std::string&);
  inline static bool subtract(Value&,const Value&,std::string&);
  inline static bool multiply(Value&,const Value&,std::string&);
  inline static bool divide(Value&,const Value&,std::string&);
    
  inline static bool if_gt(Value&,const Value&,std::string&);
  inline static bool if_eq(Value&,const Value&,std::string&);

  inline static bool if_ne(Lib::Value& aThis,const Value& aV,
                           std::string& aError) {
    if(!if_eq(aThis,aV,aError)) return false;
    aThis.u.f_bool = (aThis.u.f_bool?false:true);
    return true;
  }
  inline static bool if_ge(Lib::Value& aThis,const Value& aV,
                           std::string& aError){
    Value tmp(aThis);
    if(!if_eq(aThis,aV,aError)) return false;
    if(aThis.u.f_bool) return true;
    // then not equal, check gt :
    if(!if_gt(tmp,aV,aError)) return false;
    aThis.u.f_bool = tmp.u.f_bool;
    return true;
  }

  inline static bool if_lt(Lib::Value& aThis,const Value& aV,
                           std::string& aError){
    if(!if_ge(aThis,aV,aError)) return false;
    aThis.u.f_bool = (aThis.u.f_bool?false:true);  
    return true;
  }

  inline static bool if_le(Lib::Value& aThis,const Value& aV,
                           std::string& aError){
    if(!if_gt(aThis,aV,aError)) return false;
    aThis.u.f_bool = (aThis.u.f_bool?false:true);  
    return true;
  }

  inline static bool if_and(Value&,const Value&,std::string&);
  inline static bool if_or(Value&,const Value&,std::string&);

  inline static bool to_double(const Value&,double&);
  inline static bool cxx_type(const Value&,std::string&);

  inline static bool i_set(Value&,const Slash::Core::IValue&);
  inline static std::string toString(const Value&);
};

}

#include "Value.icc"

#endif
