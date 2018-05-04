#ifndef Slash_Data_IProcessor_h
#define Slash_Data_IProcessor_h

#include <Slash/Data/IAccessor.h> // For IAccessor::Data

#include <ostream>

namespace Slash {namespace Core { class IAction;}}
namespace Slash {namespace Core { class IWriter;}}
namespace Slash {namespace Data { class IAccessor;}}
namespace Slash {namespace Data { class IHandler;}}
namespace Slash {namespace Data { class IHandlersIterator;}}

namespace Slash {

namespace Data {

class IProcessor {
public:
  virtual ~IProcessor() {}
public:
  virtual std::ostream& out() = 0;
  virtual void setVerboseLevel(int) = 0;
  virtual int verboseLevel() const = 0;
  // Data Accessors :
  virtual void addAccessor(IAccessor*) = 0;
  virtual void removeAccessor(const std::string&) = 0;
  virtual IAccessor* findAccessor(const std::string&) = 0;
  virtual std::vector<std::string> accessorNames() const = 0;
  virtual std::vector<IAccessor*> accessors() const = 0;
  // Actions :
  virtual void addAction(Core::IAction*) = 0;
  virtual void removeAction(const std::string&) = 0;
  virtual Core::IAction* findAction(const std::string&) const = 0;
  virtual std::vector<std::string> actionNames() const = 0;
  virtual std::string execute(const std::string&,
                              const std::vector<std::string>&) = 0;
  // Handlers :
  virtual void addHandler(IAccessor*,IAccessor::Data,void*) = 0;
  virtual unsigned int numberOfHandlers() const = 0;
  virtual IHandlersIterator* handlersIterator() const = 0;
  virtual void eraseHandlers() = 0;
  virtual void addOutHandler(IAccessor*,IAccessor::Data,void*) = 0;
  virtual IHandlersIterator* outHandlersIterator() const = 0;
  virtual void eraseOutHandlers() = 0;
  virtual void swapHandlers() = 0;
  virtual std::vector<IAccessor*> handlersAccessors() const = 0;
  virtual bool type_id_tag(const std::string&,IAccessor*&,void*&,void*&) = 0;
};

} //Data

} //Slash

#endif




