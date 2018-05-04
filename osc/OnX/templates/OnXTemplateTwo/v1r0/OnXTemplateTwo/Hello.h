#ifndef OnXTemplateTwo_Hello_h
#define OnXTemplateTwo_Hello_h 

namespace Slash { namespace Core {class ISession;}}

namespace OnXTemplateTwo {

class Hello {
public:
  Hello(Slash::Core::ISession&);
  virtual ~Hello();
  void hello();
private:
  Slash::Core::ISession& fSession;
};

}

#endif

