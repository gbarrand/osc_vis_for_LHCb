#ifndef Slash_Data_IHandlersIterator_h
#define Slash_Data_IHandlersIterator_h

namespace Slash {namespace Data { class IHandler;}}

namespace Slash {

namespace Data {

class IHandlersIterator {
public:
  virtual ~IHandlersIterator() {}
public:
  virtual const IHandler* handler() const = 0;
  virtual void next() = 0;
};

} //Data

} //Slash

#endif



