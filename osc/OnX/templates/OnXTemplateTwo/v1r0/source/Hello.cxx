// this :
#include <OnXTemplateTwo/Hello.h>

// Slash :
#include <Slash/Core/ISession.h>

OnXTemplateTwo::Hello::Hello(Slash::Core::ISession& aSession)
:fSession(aSession){}

OnXTemplateTwo::Hello::~Hello() {}

void OnXTemplateTwo::Hello::hello() {
  std::ostream& out = fSession.cout();
  out << "Hello world ! (from C++ wrapped in Python)" << std::endl;
}

