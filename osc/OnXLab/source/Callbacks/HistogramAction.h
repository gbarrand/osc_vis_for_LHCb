#ifndef OnXLab_HistogramAction_h
#define OnXLab_HistogramAction_h

// Inheritance :
#include <Slash/Core/IAction.h>

namespace Slash {namespace Core {class ISession;}}

namespace AIDA {
  class IHistogram1D;
  class IHistogram2D;
}

namespace Slash {namespace Data {class IAccessor;}}
namespace Slash {namespace Data {class IProcessor;}}

namespace OnXLab {

class HistogramAction : public virtual Slash::Core::IAction {
public: //Slash::Core::IAction
  virtual void* cast(const std::string&) const;
  virtual std::string name() const;
  virtual std::string execute(const std::vector<std::string>&);
public:
  HistogramAction(Slash::Data::IProcessor&,Slash::Core::ISession&);
  virtual ~HistogramAction();
  Slash::Core::ISession& session();
  AIDA::IHistogram1D* histogram1D() const;
  AIDA::IHistogram2D* histogram2D() const;
private:
  Slash::Data::IAccessor* firstType() const;
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
  Slash::Core::ISession& fSession;
  AIDA::IHistogram1D* fHistogram1D;
  AIDA::IHistogram2D* fHistogram2D;
};

}

#endif



