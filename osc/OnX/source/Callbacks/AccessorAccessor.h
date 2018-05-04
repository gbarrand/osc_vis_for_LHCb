#ifndef OnX_AccessorAccessor_h
#define OnX_AccessorAccessor_h

// Inheritance :
#include <Slash/Tools/BaseAccessor.h>

namespace Slash {namespace Core {class ISession;}}

namespace OnX {

class AccessorAccessor : public Slash::BaseAccessor {
public:
  AccessorAccessor(Slash::Core::ISession&);
  virtual ~AccessorAccessor();
public: //Slash::Data::IAccessor
  virtual std::string name() const;
  virtual Slash::Data::IIterator* iterator();
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data,const std::string&,void*);
private:
  Slash::Core::ISession& fSession;
  std::string fName;
};

}

#endif
