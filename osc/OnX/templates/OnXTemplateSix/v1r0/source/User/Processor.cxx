// this :
#include <User/Processor.h>

User::Processor::Processor(Framework& aFramework)
:fFramework(aFramework){}

User::Processor::Processor(const Processor& aFrom):fFramework(aFrom.fFramework){}

User::Processor::~Processor() {}

User::Processor& User::Processor::operator=(const Processor&) {
  return *this;
}
