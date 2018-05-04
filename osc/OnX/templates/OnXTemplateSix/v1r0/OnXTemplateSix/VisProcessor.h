#ifndef OnXTemplateSix_VisProcessor_h
#define OnXTemplateSix_VisProcessor_h

//
// Example of simple user framework visualization service.
//

// Inheritance :
#include <User/Processor.h>

#include <string>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace UI {class IUI;}}

namespace OnXTemplateSix {

class VisProcessor : public User::Processor {
public:
  VisProcessor(User::Framework&,const std::string&);
  virtual ~VisProcessor();
private:
  VisProcessor(const VisProcessor&);
  VisProcessor& operator=(const VisProcessor&);
public:
  virtual bool isValid();
  virtual void processEvent(const User::Event&);
public:
  Slash::Core::ISession* session();
private:
  Slash::Core::ISession* fSession;
  Slash::UI::IUI* fUI;
};

}

#endif
