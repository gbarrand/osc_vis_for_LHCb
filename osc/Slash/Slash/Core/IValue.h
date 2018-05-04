#ifndef Slash_Core_IValue_h
#define Slash_Core_IValue_h

#include <string>
#include <vector>
#include <Slash/Core/typedefs.h>

namespace Slash {

namespace Core {

class IValue {
public:
  virtual ~IValue() {}
  virtual void* cast(const std::string&) const = 0;
public:
  enum Type {
    NONE = 0,
    // integers :
    UNSIGNED_CHAR = 10,
    CHAR = 11,
    UNSIGNED_SHORT = 12,
    SHORT = 13,
    UNSIGNED_INT = 14,
    INT = 15,
    UNSIGNED_INT64 = 16,
    INT64 = 17,
    // reals :
    FLOAT = 30,
    DOUBLE = 31,
    // else :
    BOOL = 50,
    STRING = 51,
    // pointers :
    VOID_STAR = 100,
    DOUBLE_STAR = 101,
    FLOAT_STAR = 102,
    INT_STAR = 103,
    // multidimensional vectors (1000+base type) :
    ARRAY_UNSIGNED_CHAR = 1010,
    ARRAY_CHAR = 1011,
    ARRAY_UNSIGNED_SHORT = 1012,
    ARRAY_SHORT = 1013,
    ARRAY_UNSIGNED_INT = 1014,
    ARRAY_INT = 1015,
    ARRAY_UNSIGNED_INT64 = 1016,
    ARRAY_INT64 = 1017,
    ARRAY_FLOAT = 1030,
    ARRAY_DOUBLE = 1031,
    ARRAY_BOOL = 1050,
    ARRAY_STRING = 1051
  };
  virtual void setType(Type) = 0;
  virtual Type type() const = 0;
  virtual std::string stype() const = 0;

  virtual unsigned char get_unsigned_char() const = 0;
  virtual char get_char() const = 0;
  virtual unsigned int get_unsigned_int() const = 0;
  virtual int get_int() const = 0;
  virtual Slash::uint64 get_unsigned_int64() const = 0;
  virtual Slash::int64 get_int64() const = 0;
  virtual unsigned short get_unsigned_short() const = 0;
  virtual short get_short() const = 0;
  virtual float get_float() const = 0;
  virtual double get_double() const = 0;
  virtual void* get_void_star() const = 0;
  virtual double* get_double_star() const = 0;
  virtual float* get_float_star() const = 0;
  virtual int* get_int_star() const = 0;
  virtual bool get_bool() const = 0;
  virtual std::string get_string() const = 0;

  virtual void setNone() = 0;
  virtual void set(unsigned char) = 0;
  virtual void set(char) = 0;
  virtual void set(unsigned short) = 0;
  virtual void set(unsigned int) = 0;
  virtual void set(int) = 0;
  virtual void set(Slash::uint64) = 0;
  virtual void set(Slash::int64) = 0;
  virtual void set(short) = 0;
  virtual void set(float) = 0;
  virtual void set(double) = 0;
  virtual void set(bool) = 0;
  virtual void set(const std::string&) = 0;
  virtual void set(const char*) = 0;
  virtual void set(void*) = 0;
  virtual void set(double*) = 0;
  virtual void set(float*) = 0;
  virtual void set(int*) = 0;

  virtual void setLabel(const std::string&) = 0;
  virtual std::string label() const = 0;

  virtual void setDimension(unsigned int) = 0;
  virtual unsigned int dimension() const = 0;

  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<unsigned char>&) = 0;
  virtual const std::vector<unsigned char>&
            get_array_unsigned_char(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<char>&) = 0;
  virtual const std::vector<char>&
            get_array_char(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<unsigned short>&) = 0;
  virtual const std::vector<unsigned short>&
            get_array_unsigned_short(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<short>&) = 0;
  virtual const std::vector<short>&
            get_array_short(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<unsigned int>&) = 0;
  virtual const std::vector<unsigned int>&
            get_array_unsigned_int(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<int>&) = 0;
  virtual const std::vector<int>&
            get_array_int(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<Slash::uint64>&) = 0;
  virtual const std::vector<Slash::uint64>&
            get_array_unsigned_int64(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<Slash::int64>&) = 0;
  virtual const std::vector<Slash::int64>&
            get_array_int64(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<float>&) = 0;
  virtual const std::vector<float>&
            get_array_float(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<double>&) = 0;
  virtual const std::vector<double>&
            get_array_double(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<bool>&) = 0;
  virtual const std::vector<bool>&
            get_array_bool(std::vector<unsigned int>&) const = 0;
  virtual void set(const std::vector<unsigned int>&,
                   const std::vector<std::string>&) = 0;
  virtual const std::vector<std::string>&
            get_array_string(std::vector<unsigned int>&) const = 0;
public:
  //helper : (used in GetValuesAction)
  virtual std::string toString() const = 0;
};

} //Core

} //Slash

#endif
