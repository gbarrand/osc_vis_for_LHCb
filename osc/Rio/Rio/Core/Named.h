#ifndef Rio_Named_h
#define Rio_Named_h

#include <Rio/Core/Object.h>

namespace Rio {

class Named : public Object {
  RIO_META_HEADER(Named)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual const std::string& name() const;
  virtual const std::string& title() const;
  virtual bool stream(IBuffer&);
public:
  Named(const IDictionary&,const std::string&,const std::string&);
  Named(const Named&);
  virtual ~Named();
  Named& operator=(const Named&);
  void setName(const std::string&);
  void setTitle(const std::string&);
protected:
  std::string fName;
  std::string fTitle;
};

}

#endif
