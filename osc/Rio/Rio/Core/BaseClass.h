#ifndef Rio_BaseClass_h
#define Rio_BaseClass_h

#include <Rio/Interfaces/IClass.h>

#include <Rio/Core/Debug.h>

namespace Rio {

class BaseClass : public virtual IClass {
public: //Rio::IClass:
  virtual const std::string& name() const {return fName;}
  virtual const std::string& inStoreName() const {return fInStoreName;}
  virtual short version() const {return fVersion;}
  virtual bool writeBuffer(IBuffer&,void*) const {return false;}
  virtual bool readBuffer(IBuffer&,void*) const {return false;}
  virtual void* createOpaque(const Arguments&) {return 0;}
public:
  BaseClass(const std::string& aName,
            const std::string& aInStoreName,
            short aVersion)
  :fName(aName)
  ,fInStoreName(aInStoreName)
  ,fVersion(aVersion){
    Debug::increment("Rio::BaseClass");
  }
  virtual ~BaseClass() {
    Debug::decrement("Rio::BaseClass");
  }
public:
  BaseClass(const BaseClass& aFrom)
  :IClass()
  ,fName(aFrom.fName)
  ,fInStoreName(aFrom.fInStoreName)
  ,fVersion(aFrom.fVersion) {
    Debug::increment("Rio::BaseClass");
  }
  BaseClass& operator=(const BaseClass& aFrom) {
    fName = aFrom.fName;
    fInStoreName = aFrom.fInStoreName;
    fVersion = aFrom.fVersion;
    return *this;
  }
private:
  std::string fName;
  std::string fInStoreName;
  short fVersion;
};

}

#endif
