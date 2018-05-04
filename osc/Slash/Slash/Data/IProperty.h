#ifndef Slash_Data_IProperty_h
#define Slash_Data_IProperty_h

#include <string>

namespace Slash {

namespace Data {

class IProperty {
public:
  virtual ~IProperty(){}
public:
  enum Type {
    NONE = 0,
    INTEGER = 1,
    DOUBLE = 2,
    STRING = 3,
    POINTER = 4,
    BOOLEAN = 5,
    INT64 = 6,
    //
    VECTOR_INTEGER = 11,
    VECTOR_DOUBLE = 12,
    VECTOR_STRING = 13,
    //VECTOR_POINTER = 14,
    VECTOR_BOOLEAN = 15
  };
  virtual std::string name() const = 0;
  virtual std::string label() const = 0;
  virtual std::string header() const = 0;
  virtual void dress(std::string&) const = 0;
  virtual Type type() const = 0;
  virtual std::string typeString() const = 0;
};

} //Data

} //Slash

#endif



