#ifndef Slash_Tools_Property_h
#define Slash_Tools_Property_h

// Inheritance :
#include <Slash/Data/IProperty.h>

#include <inlib/strip>
#include <inlib/mnmx>

namespace Slash {

class Property : public virtual Slash::Data::IProperty {
public: //IProperty
  virtual void set(const std::string& aName,Slash::Data::IProperty::Type aType,int aHead = 0,const std::string& aLabel = ""){
    fName = aName;
    fType = aType;
    switch(fType) {
    case INTEGER:fLength = aHead ? aHead : 8;break;
    case  DOUBLE:fLength = aHead ? aHead : 12;break;
    case  STRING:fLength = aHead ? aHead : 12;break;
    case POINTER:fLength = aHead ? aHead : 12;break;
    case BOOLEAN:fLength = aHead ? aHead : 5;break;
    case   INT64:fLength = aHead ? aHead : 12;break;
    case NONE:
    case VECTOR_DOUBLE:
    case VECTOR_INTEGER:
    case VECTOR_BOOLEAN:
    case VECTOR_STRING:
    default:     fLength = aHead ? aHead : 8;break;
    }
    fLabel = aLabel;
    // Set header string used by "dump" :
    int ltitle = fName.size() + fLabel.size();
    int length = inlib::mx<int>(ltitle,fLength);
    int lspace = ltitle < length ? length - ltitle : 0;
    std::string spaces(lspace,' ');
    fHeader = spaces;
    fHeader += fName;
    fHeader += fLabel;
  }

  virtual std::string name() const {return fName;}
  virtual std::string label() const {return fLabel;}
  virtual std::string header() const {return fHeader;}


  virtual Slash::Data::IProperty::Type type() const {return fType;}

  virtual std::string typeString() const {
    // Same logic as Variable::type().
    switch(fType) {
    case INTEGER:
      return "INTEGER";
    case DOUBLE:
      return "DOUBLE";
    case STRING:
      return "STRING";
    case POINTER:
      return "POINTER";
    case BOOLEAN:
      return "BOOLEAN";
    case INT64:
      return "INT64";
    case VECTOR_DOUBLE:
      return "VECTOR_DOUBLE";
    case VECTOR_INTEGER:
      return "VECTOR_INTEGER";
    case VECTOR_BOOLEAN:
      return "VECTOR_BOOLEAN";
    case VECTOR_STRING:
      return "VECTOR_STRING";
    case NONE:
    default:
      return "NONE";
    }
  }
  
  virtual void dress(std::string& aString) const {
    inlib::strip(aString);
    int lstring = aString.length();
    int lheader = fHeader.length();
    if(lstring < lheader) {
      int lspace = lheader - lstring;
      std::string s(lspace,' ');
      s += aString;
      s.resize(lheader);
      aString = s;
    } else {
      aString.resize(lheader);
    }
  }

public:
  inline Property():fType(NONE){
    //Lib::Debug::increment();
  }
  inline Property(const std::string& aName,Slash::Data::IProperty::Type aType,int aHead = 0,const std::string& aLabel = ""){
    //Debug::increment();
    set(aName,aType,aHead,aLabel);
  }
  virtual ~Property(){
    //Debug::decrement();
  }
public:
  inline Property(const Property& aProperty):IProperty() {
    //Debug::increment();
    fName = aProperty.fName;
    fType = aProperty.fType;
    fLength = aProperty.fLength;
    fLabel = aProperty.fLabel;
    fHeader = aProperty.fHeader;
  }

  inline Property& operator=(const Property& aProperty){
    fName = aProperty.fName;
    fType = aProperty.fType;
    fLength = aProperty.fLength;
    fLabel = aProperty.fLabel;
    fHeader = aProperty.fHeader;
    return *this;
  }

private:
  std::string fName;
  Slash::Data::IProperty::Type fType;
  int fLength;
  std::string fLabel;
  std::string fHeader;
};

}

#endif



