#ifndef Slash_Tools_Value_h
#define Slash_Tools_Value_h

// Inheritance :
#include <Slash/Core/IValue.h>

#include <inlib/cast>
#include <inlib/array>
#include <inlib/sprintf> //toString
#include <inlib/tos> //toString

namespace Slash {

class Value : public virtual Slash::Core::IValue {
public: //IValue
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Core::IValue)
    else INLIB_IF_CAST(Slash::Value)
    else return 0;
  }
  virtual Slash::Core::IValue::Type type() const {return fType;}

  virtual void setType(Type aType) {
    reset();
    fType = aType;
    switch(aType) {
    case NONE:      u.f_unsigned_int64 = 0;break;
    case UNSIGNED_CHAR:    u.f_unsigned_char = 0;break;
    case CHAR:      u.f_char = 0;break;
    case UNSIGNED_SHORT:   u.f_unsigned_short = 0;break;
    case SHORT:     u.f_short = 0;break;
    case UNSIGNED_INT:     u.f_unsigned_int = 0;break;
    case INT:       u.f_int = 0;break;
    case UNSIGNED_INT64:   u.f_unsigned_int64 =0;break;
    case INT64:     u.f_int64 = 0;break;
    case FLOAT:     u.f_float = 0;break;
    case DOUBLE:    u.f_double = 0;break;
    case BOOL:      u.f_bool = false;break;
    case VOID_STAR:        u.f_void_star = 0;break;
    case DOUBLE_STAR:      u.f_double_star = 0;break;
    case FLOAT_STAR:       u.f_float_star = 0;break;
    case INT_STAR:         u.f_int_star = 0;break;
    case STRING:       u.f_string = new std::string("");break;
    case ARRAY_UNSIGNED_CHAR:
      u.f_array_unsigned_char = new inlib::array<unsigned char>();break;
    case ARRAY_CHAR:u.f_array_char = new inlib::array<char>();break;
  
    case ARRAY_UNSIGNED_SHORT:
      u.f_array_unsigned_short = new inlib::array<unsigned short>();break;
    case ARRAY_SHORT:u.f_array_short = new inlib::array<short>();break;
  
    case ARRAY_UNSIGNED_INT:
      u.f_array_unsigned_int = new inlib::array<unsigned int>();break;
    case ARRAY_INT:u.f_array_int = new inlib::array<int>();break;
    case ARRAY_UNSIGNED_INT64:
      u.f_array_unsigned_int64 = new inlib::array<Slash::uint64>();break;
    case ARRAY_INT64:u.f_array_int64 = new inlib::array<Slash::int64>();break;
  
    case ARRAY_FLOAT:u.f_array_float = new inlib::array<float>();break;
    case ARRAY_DOUBLE:u.f_array_double = new inlib::array<double>();break;
    case ARRAY_BOOL:u.f_array_bool = new inlib::array<bool>();break;
    case ARRAY_STRING:u.f_array_string = new inlib::array<std::string>();break;
    }
  }

  virtual std::string stype() const {
    // Used in Lib/AccessorManager, OnXLab/TupleAction, OnXLab/HistogramAction.
    return stype(fType);
  }
  virtual unsigned char get_unsigned_char() const {return u.f_unsigned_char;}
  virtual char get_char() const {return u.f_char;}
  virtual unsigned int get_unsigned_int() const {return u.f_unsigned_int;}
  virtual int get_int() const {return u.f_int;}

  virtual Slash::int64 get_int64() const {return u.f_int64;}
  virtual Slash::uint64 get_unsigned_int64() const {return u.f_unsigned_int64;}

  virtual unsigned short get_unsigned_short() const{return u.f_unsigned_short;}
  virtual short get_short() const {return u.f_short;}
  virtual float get_float() const {return u.f_float;}
  virtual double get_double() const {return u.f_double;}
  virtual void* get_void_star() const {return u.f_void_star;}
  virtual double* get_double_star() const {return u.f_double_star;}
  virtual float* get_float_star() const {return u.f_float_star;}
  virtual int* get_int_star() const {return u.f_int_star;}
  virtual bool get_bool() const {return u.f_bool;}
  virtual std::string get_string() const {return *u.f_string;}

  virtual void setNone() {
    reset();
    fType = NONE;
    u.f_unsigned_int64 = 0;
  }
  virtual void set(char aValue) {
    reset();
    fType = CHAR;
    u.f_char = aValue;
  }
  virtual void set(short aValue) {
    reset();
    fType = SHORT;
    u.f_short = aValue;
  }
  virtual void set(int aValue) {
    reset();
    fType = INT;
    u.f_int = aValue;
  }
  virtual void set(float aValue) {
    reset();
    fType = FLOAT;
    u.f_float = aValue;
  }
  virtual void set(double aValue) {
    reset();
    fType = DOUBLE;
    u.f_double = aValue;
  }
  virtual void set(unsigned char aValue) {
    reset();
    fType = UNSIGNED_CHAR;
    u.f_unsigned_char = aValue;
  }
  virtual void set(unsigned short aValue) {
    reset();
    fType = UNSIGNED_SHORT;
    u.f_unsigned_short  = aValue;
  }
  virtual void set(unsigned int aValue) {
    reset();
    fType = UNSIGNED_INT;
    u.f_unsigned_int  = aValue;
  }
  virtual void set(Slash::int64 aValue) {
    reset();
    fType = INT64;
    u.f_int64 = aValue;
  }
  virtual void set(Slash::uint64 aValue) {
    reset();
    fType = UNSIGNED_INT64;
    u.f_unsigned_int64 = aValue;
  }
  virtual void set(bool aValue) {
    reset();
    fType = BOOL;
    u.f_bool = aValue;
  }

  virtual void set(const std::string& aValue) {
    reset();
    fType = STRING;
    u.f_string = new std::string(aValue);
  }
  virtual void set(const char* aValue) {
    // To avoid the bool, const std::string& and passing "text" problem.
    reset();
    fType = STRING;
    u.f_string = new std::string(aValue);
  }

  virtual void set(void* aValue) {
    reset();
    fType = VOID_STAR;
    u.f_void_star = aValue;
  }
  virtual void set(double* aValue) {
    reset();
    fType = DOUBLE_STAR;
    u.f_double_star = aValue;
  }
  virtual void set(float* aValue) {
    reset();
    fType = FLOAT_STAR;
    u.f_float_star = aValue;
  }
  virtual void set(int* aValue) {
    reset();
    fType = INT_STAR;
    u.f_int_star = aValue;
  }

#define SLASH_VALUE_SET(a_what,a_f_what,a_type) \
  virtual void set(const std::vector<unsigned int>& aOrders,\
                   const std::vector<a_what>& aValues){\
    reset();\
    fType = a_type;\
    u.a_f_what = new inlib::array<a_what>(aOrders);\
    u.a_f_what->fill(aValues);\
  }

  SLASH_VALUE_SET(unsigned char,f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
  SLASH_VALUE_SET(char,f_array_char,ARRAY_CHAR)
  SLASH_VALUE_SET(unsigned short,f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
  SLASH_VALUE_SET(short,f_array_short,ARRAY_SHORT)
  SLASH_VALUE_SET(unsigned int,f_array_unsigned_int,ARRAY_UNSIGNED_INT)
  SLASH_VALUE_SET(int,f_array_int,ARRAY_INT)
  SLASH_VALUE_SET(Slash::uint64,f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
  SLASH_VALUE_SET(Slash::int64,f_array_int64,ARRAY_INT64)
  SLASH_VALUE_SET(float,f_array_float,ARRAY_FLOAT)
  SLASH_VALUE_SET(double,f_array_double,ARRAY_DOUBLE)
  SLASH_VALUE_SET(bool,f_array_bool,ARRAY_BOOL)
  SLASH_VALUE_SET(std::string,f_array_string,ARRAY_STRING)

#undef SLASH_VALUE_SET

#define SLASH_VALUE_GET(a_what,a_get_what,a_f_what) \
  virtual const std::vector<a_what>& a_get_what(std::vector<unsigned int>& aOrders) const {\
    aOrders = u.a_f_what->orders();\
    return u.a_f_what->vector();\
  }

  SLASH_VALUE_GET(unsigned char,get_array_unsigned_char,f_array_unsigned_char)
  SLASH_VALUE_GET(char,get_array_char,f_array_char)
  SLASH_VALUE_GET(unsigned short,get_array_unsigned_short,f_array_unsigned_short)
  SLASH_VALUE_GET(short,get_array_short,f_array_short)
  SLASH_VALUE_GET(unsigned int,get_array_unsigned_int,f_array_unsigned_int)
  SLASH_VALUE_GET(int,get_array_int,f_array_int)
  SLASH_VALUE_GET(Slash::uint64,get_array_unsigned_int64,f_array_unsigned_int64)
  SLASH_VALUE_GET(Slash::int64,get_array_int64,f_array_int64)
  SLASH_VALUE_GET(float,get_array_float,f_array_float)
  SLASH_VALUE_GET(double,get_array_double,f_array_double)
  SLASH_VALUE_GET(bool,get_array_bool,f_array_bool)
  SLASH_VALUE_GET(std::string,get_array_string,f_array_string)

#undef SLASH_VALUE_GET

  virtual void setLabel(const std::string& aString) {
    delete fLabel;
    fLabel = new std::string(aString);
  }
  virtual std::string label() const {return fLabel?(*fLabel):"";}
  virtual void setDimension(unsigned int aDimension) {
    fDimension = aDimension;
  }
  virtual unsigned int dimension() const {return fDimension;}

public:
  // Not (yet) in IValue. Used in BatchLab MemoryTuple.
#define SLASH_VALUE_SET1D(a_what,a_f_what,a_type)	\
  virtual void set(const std::vector<a_what>& aValues){\
    reset();\
    fType = a_type;\
    std::vector<unsigned int> orders(1);\
    orders[0] = aValues.size();\
    u.a_f_what = new inlib::array<a_what>(orders);\
    u.a_f_what->fill(aValues);\
  }

  SLASH_VALUE_SET1D(unsigned char,f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
  SLASH_VALUE_SET1D(char,f_array_char,ARRAY_CHAR)
  SLASH_VALUE_SET1D(unsigned short,f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
  SLASH_VALUE_SET1D(short,f_array_short,ARRAY_SHORT)
  SLASH_VALUE_SET1D(unsigned int,f_array_unsigned_int,ARRAY_UNSIGNED_INT)
  SLASH_VALUE_SET1D(int,f_array_int,ARRAY_INT)
  SLASH_VALUE_SET1D(Slash::uint64,f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
  SLASH_VALUE_SET1D(Slash::int64,f_array_int64,ARRAY_INT64)
  SLASH_VALUE_SET1D(float,f_array_float,ARRAY_FLOAT)
  SLASH_VALUE_SET1D(double,f_array_double,ARRAY_DOUBLE)
  SLASH_VALUE_SET1D(bool,f_array_bool,ARRAY_BOOL)
  SLASH_VALUE_SET1D(std::string,f_array_string,ARRAY_STRING)

#undef SLASH_VALUE_SET1D

public:
  //helpers :
  virtual std::string toString() const {
    // used in Lib/GetValuesAction::execute.
    std::string s;
    switch(fType) {
    case Slash::Core::IValue::NONE:
      inlib::sprintf(s,5,"(nil)");
      return s;
    case Slash::Core::IValue::INT:
      inlib::sprintf(s,16,"%d",u.f_int);
      return s;
    case Slash::Core::IValue::DOUBLE:
      inlib::sprintf(s,16,"%g",u.f_double);
      return s;
    case Slash::Core::IValue::VOID_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_void_star);
      return s;
    case Slash::Core::IValue::UNSIGNED_SHORT:
      inlib::sprintf(s,16,"%u",u.f_unsigned_short);
      return s;
    case Slash::Core::IValue::UNSIGNED_INT:
      inlib::sprintf(s,16,"%u",u.f_unsigned_int);
      return s;
    case Slash::Core::IValue::BOOL:
      inlib::sprintf(s,5,"%s",u.f_bool?"true":"false");
      return s;
    case Slash::Core::IValue::SHORT:
      inlib::sprintf(s,16,"%d",u.f_short);
      return s;
    case Slash::Core::IValue::INT64:{
      inlib::sprintf(s,16,inlib::int64_format(),u.f_int64);
     }return s;
    case Slash::Core::IValue::UNSIGNED_INT64:{
      inlib::sprintf(s,16,inlib::int64_format(),u.f_unsigned_int64);
     }return s;
    case Slash::Core::IValue::FLOAT:
      inlib::sprintf(s,16,"%g",u.f_float);
      return s;
    case Slash::Core::IValue::UNSIGNED_CHAR:
      inlib::sprintf(s,16,"%c",u.f_unsigned_char);
      return s;
    case Slash::Core::IValue::CHAR:
      inlib::sprintf(s,16,"%c",u.f_char);
      return s;
    case Slash::Core::IValue::DOUBLE_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_double_star);
      return s;
    case Slash::Core::IValue::FLOAT_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_float_star);
      return s;
    case Slash::Core::IValue::INT_STAR:
      inlib::sprintf(s,16,"0x%lx",(unsigned long)u.f_int_star);
      return s;
    case Slash::Core::IValue::STRING:
      return *u.f_string;

#define SLASH_VALUE_TOS_ARRAY(a_what,a_f_what,a_type) \
    case Slash::Core::IValue::a_type:\
     {const std::vector<a_what>& stdv = u.a_f_what->vector();\
      unsigned int number = stdv.size();\
      for(unsigned int index=0;index<number;index++) {\
        if(index) s+= "\n";\
        s += inlib::tos(stdv[index]);\
      }}\
      return s;

    SLASH_VALUE_TOS_ARRAY(unsigned char,f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_TOS_ARRAY(char,f_array_char,ARRAY_CHAR)
    SLASH_VALUE_TOS_ARRAY(unsigned short,f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_TOS_ARRAY(short,f_array_short,ARRAY_SHORT)
    SLASH_VALUE_TOS_ARRAY(unsigned int,f_array_unsigned_int,ARRAY_UNSIGNED_INT)
    SLASH_VALUE_TOS_ARRAY(int,f_array_int,ARRAY_INT)
    SLASH_VALUE_TOS_ARRAY(Slash::uint64,f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_TOS_ARRAY(Slash::int64,f_array_int64,ARRAY_INT64)
    SLASH_VALUE_TOS_ARRAY(float,f_array_float,ARRAY_FLOAT)
    SLASH_VALUE_TOS_ARRAY(double,f_array_double,ARRAY_DOUBLE)
    SLASH_VALUE_TOS_ARRAY(bool,f_array_bool,ARRAY_BOOL)
    SLASH_VALUE_TOS_ARRAY(std::string,f_array_string,ARRAY_STRING)
    }

    return "(nil)";
  }

public:
  inline Value():fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = NONE;
    u.f_unsigned_int64 = 0;
  }
  inline Value(bool aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = BOOL;
    u.f_bool = aValue;
  }
  inline Value(char aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = CHAR;
    u.f_char = aValue;
  }
  inline Value(short aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = SHORT;
    u.f_short = aValue;
  }
  inline Value(int aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = INT;
    u.f_int = aValue;
  }
  inline Value(Slash::int64 aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = INT64;
    u.f_int64 = aValue;
  }
  inline Value(Slash::uint64 aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = UNSIGNED_INT64;
    u.f_unsigned_int64 = aValue;
  }
  inline Value(float aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = FLOAT;
    u.f_float = aValue;
  }
  inline Value(double aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = DOUBLE;
    u.f_double = aValue;
  }

  inline Value(unsigned char aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = UNSIGNED_CHAR;
    u.f_unsigned_char = aValue;
  }
  inline Value(unsigned short aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = UNSIGNED_SHORT;
    u.f_unsigned_short = aValue;
  }
  inline Value(unsigned int aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = UNSIGNED_INT;
    u.f_unsigned_int = aValue;
  }
  inline Value(void* aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = VOID_STAR;
    u.f_void_star = aValue;
  }
  inline Value(double* aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = DOUBLE_STAR;
    u.f_double_star = aValue;
  }
  inline Value(float* aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = FLOAT_STAR;
    u.f_float_star = aValue;
  }
  inline Value(int* aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = INT_STAR;
    u.f_int_star = aValue;
  }
  inline Value(const char* aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = STRING;
    u.f_string = new std::string(aValue?aValue:"");
  }
  inline Value(const std::string& aValue):fLabel(0),fDimension(0) {
    //Debug::increment("Lib::Value");
    fType = STRING;
    u.f_string = new std::string(aValue);
  }

  inline Value(const std::vector<double>& aValue):fLabel(0),fDimension(0){
    //Debug::increment("Lib::Value");
    fType = ARRAY_DOUBLE;
    std::vector<unsigned int> is(1);
    is[0] = aValue.size();
    u.f_array_double = new inlib::array<double>(is);
    u.f_array_double->fill(aValue);
  }

#define SLASH_VALUE_CSTOR(a_what,a_f_what,a_type) \
  inline Value(const std::vector<unsigned int>& aOrders,\
               const std::vector<a_what>& aValues)\
  :fLabel(0),fDimension(0){\
    /*Debug::increment("Lib::Value");*/\
    fType = a_type;\
    u.a_f_what = new inlib::array<a_what>(aOrders);\
    u.a_f_what->fill(aValues);\
  }

  SLASH_VALUE_CSTOR(unsigned char,f_array_unsigned_char,Slash::Core::IValue::ARRAY_UNSIGNED_CHAR)
  SLASH_VALUE_CSTOR(char,f_array_char,Slash::Core::IValue::ARRAY_CHAR)
  SLASH_VALUE_CSTOR(unsigned short,f_array_unsigned_short,Slash::Core::IValue::ARRAY_UNSIGNED_SHORT)
  SLASH_VALUE_CSTOR(short,f_array_short,Slash::Core::IValue::ARRAY_SHORT)
  SLASH_VALUE_CSTOR(unsigned int,f_array_unsigned_int,Slash::Core::IValue::ARRAY_UNSIGNED_INT)
  SLASH_VALUE_CSTOR(int,f_array_int,Slash::Core::IValue::ARRAY_INT)
  SLASH_VALUE_CSTOR(Slash::uint64,f_array_unsigned_int64,Slash::Core::IValue::ARRAY_UNSIGNED_INT64)
  SLASH_VALUE_CSTOR(Slash::int64,f_array_int64,Slash::Core::IValue::ARRAY_INT64)
  SLASH_VALUE_CSTOR(float,f_array_float,Slash::Core::IValue::ARRAY_FLOAT)
  SLASH_VALUE_CSTOR(double,f_array_double,Slash::Core::IValue::ARRAY_DOUBLE)
  SLASH_VALUE_CSTOR(bool,f_array_bool,Slash::Core::IValue::ARRAY_BOOL)
  SLASH_VALUE_CSTOR(std::string,f_array_string,Slash::Core::IValue::ARRAY_STRING)

#undef SLASH_VALUE_CSTOR

  virtual ~Value(){ 
    delete fLabel;
    reset();
    //Debug::decrement("Lib::Value");
  }

public:

  inline Value(const Value& aFrom):IValue(),fLabel(0){
    //Debug::increment("Lib::Value");

    if(aFrom.fLabel) fLabel = new std::string(*aFrom.fLabel);
    fDimension = aFrom.fDimension;
    fType = aFrom.fType;

    if(aFrom.fType==STRING) {
      u.f_string = new std::string(*aFrom.u.f_string);

#define SLASH_VALUE_CP(a_what,a_f_what,a_type) \
    } else if(aFrom.fType==a_type) {\
      u.a_f_what = new inlib::array<a_what>(*aFrom.u.a_f_what);


    SLASH_VALUE_CP(unsigned char,f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_CP(char,f_array_char,ARRAY_CHAR)
    SLASH_VALUE_CP(unsigned short,f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_CP(short,f_array_short,ARRAY_SHORT)
    SLASH_VALUE_CP(unsigned int,f_array_unsigned_int,ARRAY_UNSIGNED_INT)
    SLASH_VALUE_CP(int,f_array_int,ARRAY_INT)
    SLASH_VALUE_CP(Slash::uint64,f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_CP(Slash::int64,f_array_int64,ARRAY_INT64)
    SLASH_VALUE_CP(float,f_array_float,ARRAY_FLOAT)
    SLASH_VALUE_CP(double,f_array_double,ARRAY_DOUBLE)
    SLASH_VALUE_CP(bool,f_array_bool,ARRAY_BOOL)
    SLASH_VALUE_CP(std::string,f_array_string,ARRAY_STRING)

    } else {
      u = aFrom.u;
    }
  }

  inline Value& operator=(const Value& aFrom) {
    delete fLabel;
    fLabel = 0;
    if(aFrom.fLabel) fLabel = new std::string(*aFrom.fLabel);
    fDimension = aFrom.fDimension;
    reset();
    fType = aFrom.fType;

    if(aFrom.fType==STRING) {
      u.f_string = new std::string(*aFrom.u.f_string);

    SLASH_VALUE_CP(unsigned char,f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_CP(char,f_array_char,ARRAY_CHAR)
    SLASH_VALUE_CP(unsigned short,f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_CP(short,f_array_short,ARRAY_SHORT)
    SLASH_VALUE_CP(unsigned int,f_array_unsigned_int,ARRAY_UNSIGNED_INT)
    SLASH_VALUE_CP(int,f_array_int,ARRAY_INT)
    SLASH_VALUE_CP(Slash::uint64,f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_CP(Slash::int64,f_array_int64,ARRAY_INT64)
    SLASH_VALUE_CP(float,f_array_float,ARRAY_FLOAT)
    SLASH_VALUE_CP(double,f_array_double,ARRAY_DOUBLE)
    SLASH_VALUE_CP(bool,f_array_bool,ARRAY_BOOL)
    SLASH_VALUE_CP(std::string,f_array_string,ARRAY_STRING)
  
    } else {
      u = aFrom.u;
    }

    return *this;
  }

#undef SLASH_VALUE_CP

public:
  inline bool copy(const Slash::Core::IValue& aValue) {
    // used in OnXLab/source/Callbacks/TupleAction.cxx
    // used in Lib/GetValuesAction::execute.
    reset();

    fType = aValue.type();

    switch(aValue.type()) {
    case Slash::Core::IValue::NONE:
      u.f_unsigned_int64 = 0;
      return true;
    case Slash::Core::IValue::CHAR:
      u.f_char = aValue.get_char();
      return true;
    case Slash::Core::IValue::SHORT:
      u.f_short = aValue.get_short();
      return true;
    case Slash::Core::IValue::INT:
      u.f_int = aValue.get_int();
      return true;
    case Slash::Core::IValue::INT64:
      u.f_int64 = aValue.get_int64();
      return true;
    case Slash::Core::IValue::UNSIGNED_INT64:
      u.f_unsigned_int64 = aValue.get_unsigned_int64();
      return true;
    case Slash::Core::IValue::UNSIGNED_CHAR:
      u.f_unsigned_char = aValue.get_unsigned_char();
      return true;
    case Slash::Core::IValue::UNSIGNED_SHORT:
      u.f_unsigned_short  = aValue.get_unsigned_short();
      return true;
    case Slash::Core::IValue::UNSIGNED_INT:
      u.f_unsigned_int  = aValue.get_unsigned_int();
      return true;
    case Slash::Core::IValue::DOUBLE_STAR:
      u.f_double_star  = aValue.get_double_star();
      return true;
    case Slash::Core::IValue::FLOAT_STAR:
      u.f_float_star  = aValue.get_float_star();
      return true;
    case Slash::Core::IValue::INT_STAR:
      u.f_int_star  = aValue.get_int_star();
      return true;
    case Slash::Core::IValue::FLOAT:
      u.f_float = aValue.get_float();
      return true;
    case Slash::Core::IValue::DOUBLE:
      u.f_double = aValue.get_double();
      return true;
    case Slash::Core::IValue::VOID_STAR:
      u.f_void_star = aValue.get_void_star();
      return true;
    case Slash::Core::IValue::BOOL:
      u.f_bool = aValue.get_bool();
      return true;
    case Slash::Core::IValue::STRING:
      u.f_string = new std::string(aValue.get_string());
      return true;

#define SLASH_VALUE_COPY_CASE(a_what,a_get_what,a_f_what,a_type) \
    case Slash::Core::IValue::a_type:\
     {std::vector<unsigned int> is;\
      const std::vector<a_what>& vd = aValue.a_get_what(is);\
      u.a_f_what = new inlib::array<a_what>();	  \
      if(!u.a_f_what->configure(is)) {\
        setNone();\
        return false;\
      }\
      if(!u.a_f_what->fill(vd)) {\
        setNone();\
        return false;\
      }\
      return true;}
  
    SLASH_VALUE_COPY_CASE(unsigned char,get_array_unsigned_char,f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_COPY_CASE(char,get_array_char,f_array_char,ARRAY_CHAR)
    SLASH_VALUE_COPY_CASE(unsigned short,get_array_unsigned_short,f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_COPY_CASE(short,get_array_short,f_array_short,ARRAY_SHORT)
    SLASH_VALUE_COPY_CASE(unsigned int,get_array_unsigned_int,f_array_unsigned_int,ARRAY_UNSIGNED_INT)
    SLASH_VALUE_COPY_CASE(int,get_array_int,f_array_int,ARRAY_INT)
    SLASH_VALUE_COPY_CASE(Slash::uint64,get_array_unsigned_int64,f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_COPY_CASE(Slash::int64,get_array_int64,f_array_int64,ARRAY_INT64)
    SLASH_VALUE_COPY_CASE(float,get_array_float,f_array_float,ARRAY_FLOAT)
    SLASH_VALUE_COPY_CASE(double,get_array_double,f_array_double,ARRAY_DOUBLE)
    SLASH_VALUE_COPY_CASE(bool,get_array_bool,f_array_bool,ARRAY_BOOL)
    SLASH_VALUE_COPY_CASE(std::string,get_array_string,f_array_string,ARRAY_STRING)

#undef SLASH_VALUE_COPY_CASE

    }

    setNone();
    return false;
  }

public:
  // For Expression :
  virtual std::vector<double>& array_double() {
    return u.f_array_double->vector(); 
  }

  inline static std::string stype(Slash::Core::IValue::Type aType){

#define SLASH_VALUE_CASE(a) case a:return #a;
    switch(aType) {
    SLASH_VALUE_CASE(NONE)
    SLASH_VALUE_CASE(INT)
    SLASH_VALUE_CASE(INT64)
    SLASH_VALUE_CASE(DOUBLE)
    SLASH_VALUE_CASE(STRING)
    SLASH_VALUE_CASE(VOID_STAR)
    SLASH_VALUE_CASE(DOUBLE_STAR)
    SLASH_VALUE_CASE(FLOAT_STAR)
    SLASH_VALUE_CASE(INT_STAR)
    SLASH_VALUE_CASE(BOOL)
    SLASH_VALUE_CASE(SHORT)
    SLASH_VALUE_CASE(FLOAT)
    SLASH_VALUE_CASE(CHAR)
    SLASH_VALUE_CASE(UNSIGNED_CHAR)
    SLASH_VALUE_CASE(UNSIGNED_SHORT)
    SLASH_VALUE_CASE(UNSIGNED_INT)
    SLASH_VALUE_CASE(UNSIGNED_INT64)
    SLASH_VALUE_CASE(ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_CASE(ARRAY_CHAR)
    SLASH_VALUE_CASE(ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_CASE(ARRAY_SHORT)
    SLASH_VALUE_CASE(ARRAY_UNSIGNED_INT)
    SLASH_VALUE_CASE(ARRAY_INT)
    SLASH_VALUE_CASE(ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_CASE(ARRAY_INT64)
    SLASH_VALUE_CASE(ARRAY_FLOAT)
    SLASH_VALUE_CASE(ARRAY_DOUBLE)
    SLASH_VALUE_CASE(ARRAY_BOOL)
    SLASH_VALUE_CASE(ARRAY_STRING)
    }
#undef SLASH_VALUE_CASE

    return "unknown";
  }

  
  //used in Lib::Columns.y :
  inline static bool arrayType(Slash::Core::IValue::Type aFrom,
                               Slash::Core::IValue::Type& aTo){
    //aFrom is a non array type.
#define SLASH_VALUE_ACASE(a_from,a_to)\
    case a_from:\
      aTo = Slash::Core::IValue::a_to;\
      return true;

    switch(aFrom) {

    SLASH_VALUE_ACASE(UNSIGNED_CHAR,ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_ACASE(CHAR,ARRAY_CHAR)
    SLASH_VALUE_ACASE(UNSIGNED_SHORT,ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_ACASE(SHORT,ARRAY_SHORT)
    SLASH_VALUE_ACASE(UNSIGNED_INT,ARRAY_UNSIGNED_INT)
    SLASH_VALUE_ACASE(INT,ARRAY_INT)
    SLASH_VALUE_ACASE(UNSIGNED_INT64,ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_ACASE(INT64,ARRAY_INT64)
    SLASH_VALUE_ACASE(BOOL,ARRAY_BOOL)
    SLASH_VALUE_ACASE(DOUBLE,ARRAY_DOUBLE)
    SLASH_VALUE_ACASE(FLOAT,ARRAY_FLOAT)
    SLASH_VALUE_ACASE(STRING,ARRAY_STRING)

    case VOID_STAR:
    case DOUBLE_STAR:
    case FLOAT_STAR:
    case INT_STAR:
  
    case ARRAY_UNSIGNED_CHAR:
    case ARRAY_CHAR:
    case ARRAY_UNSIGNED_SHORT:
    case ARRAY_SHORT:
    case ARRAY_UNSIGNED_INT:
    case ARRAY_INT:
    case ARRAY_UNSIGNED_INT64:
    case ARRAY_INT64:
    case ARRAY_FLOAT:
    case ARRAY_DOUBLE:
    case ARRAY_BOOL:
    case ARRAY_STRING:
    case NONE:
      aTo = NONE;
      return false;
    }
#undef SLASH_VALUE_ACASE
    return false;
  }

protected:

  void reset() {
    //FIXME : fDimension = 0; ???
    if(fType==STRING) {
      delete u.f_string;
      u.f_string = 0;
  
#define SLASH_VALUE_RESET(a_f_what,a_type) \
    } else if(fType==a_type) {\
      delete u.a_f_what;\
      u.a_f_what = 0;

    SLASH_VALUE_RESET(f_array_unsigned_char,ARRAY_UNSIGNED_CHAR)
    SLASH_VALUE_RESET(f_array_char,ARRAY_CHAR)
    SLASH_VALUE_RESET(f_array_unsigned_short,ARRAY_UNSIGNED_SHORT)
    SLASH_VALUE_RESET(f_array_short,ARRAY_SHORT)
    SLASH_VALUE_RESET(f_array_unsigned_int,ARRAY_UNSIGNED_INT)
    SLASH_VALUE_RESET(f_array_int,ARRAY_INT)
    SLASH_VALUE_RESET(f_array_unsigned_int64,ARRAY_UNSIGNED_INT64)
    SLASH_VALUE_RESET(f_array_int64,ARRAY_INT64)
    SLASH_VALUE_RESET(f_array_float,ARRAY_FLOAT)
    SLASH_VALUE_RESET(f_array_double,ARRAY_DOUBLE)
    SLASH_VALUE_RESET(f_array_bool,ARRAY_BOOL)
    SLASH_VALUE_RESET(f_array_string,ARRAY_STRING)
  
#undef SLASH_VALUE_RESET

    } else {
      u.f_unsigned_int64 = 0;
    }
  }
  

private:
  std::string* fLabel;
  unsigned int fDimension;
protected:
  Type fType;
  union {
    bool f_bool;
    char f_char;
    int f_int;
    short f_short;
    Slash::int64 f_int64;
    float f_float;
    double f_double;
    unsigned char f_unsigned_char;
    unsigned short f_unsigned_short;
    unsigned int f_unsigned_int;
    Slash::uint64 f_unsigned_int64;
    void* f_void_star;
    double* f_double_star;
    float* f_float_star;
    int* f_int_star;
    std::string* f_string;

    inlib::array<unsigned char>* f_array_unsigned_char;
    inlib::array<char>* f_array_char;
    inlib::array<unsigned short>* f_array_unsigned_short;
    inlib::array<short>* f_array_short;
    inlib::array<Slash::uint64>* f_array_unsigned_int64;
    inlib::array<Slash::int64>* f_array_int64;
    inlib::array<unsigned int>* f_array_unsigned_int;
    inlib::array<int>* f_array_int;
    inlib::array<float>* f_array_float;
    inlib::array<double>* f_array_double;
    inlib::array<bool>* f_array_bool;
    inlib::array<std::string>* f_array_string;
  } u;

  //friend class valuemanip;
};

}

#endif
