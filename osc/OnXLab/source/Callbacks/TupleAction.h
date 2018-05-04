#ifndef OnXLab_TupleAction_h
#define OnXLab_TupleAction_h

// Inheritance :
#include <Slash/Core/IAction.h>

namespace Slash {namespace Core {class ISession;}}

namespace AIDA {
  class ITuple;
}

namespace Slash {namespace Data {class IAccessor;}}
namespace Slash {namespace Data {class IProcessor;}}

namespace OnXLab {

class TupleAction : public virtual Slash::Core::IAction {
public: //Slash::Core::IAction
  virtual void* cast(const std::string&) const;
  virtual std::string name() const;
  virtual std::string execute(const std::vector<std::string>&);
public:
  TupleAction(Slash::Data::IProcessor&,Slash::Core::ISession&);
  virtual ~TupleAction();
  Slash::Core::ISession& session();
  AIDA::ITuple* tuple() const;
private:
  Slash::Data::IAccessor* firstType() const;
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
  Slash::Core::ISession& fSession;
  AIDA::ITuple* fTuple;
};

}

#endif



