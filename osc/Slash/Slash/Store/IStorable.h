#ifndef Slash_Store_IStorable_h
#define Slash_Store_IStorable_h

#include <string>

namespace Slash {namespace Store { class IConstVisitor;}}
namespace Slash {namespace Store { class IVisitor;}}

namespace Slash {

namespace Store {

class IStorable {
public:
  virtual ~IStorable() {}
public:
  virtual void* cast(const std::string&) const = 0;
public:
  virtual bool visit(IConstVisitor&) const = 0;
  virtual bool read(IVisitor&) = 0;
};

} //Store

} //Slash

#define SLASH_STORE_HEADER \
private:\
  static bool s_visit(const Slash::Store::IStorable&,\
                      Slash::Store::IConstVisitor&);

#include <inlib/cast>

#define SLASH_STORE_SOURCE(aClass) \
bool aClass::s_visit(\
 const Slash::Store::IStorable& aObject\
,Slash::Store::IConstVisitor& aVisitor\
){\
  const aClass* local = INLIB_CONST_CAST(aObject,aClass);\
  if(!local) {\
    /*fprintf(stderr,"debug : %s::visit cast %ld failed\n",#aClass,local);*/\
    return false;\
  }\
  return local->aClass::visit(aVisitor);\
}

#define SLASH_STORE_BEGIN(aClass) \
  if(!aVisitor.begin(*this,#aClass,aClass::s_visit)) return false;

#endif
