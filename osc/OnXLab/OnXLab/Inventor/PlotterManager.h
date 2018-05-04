#ifndef OnXLab_PlotterManager_h
#define OnXLab_PlotterManager_h

//Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/IPlotterManager.h>

#include <vector>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace UI {class IUI;}}

namespace OnXLab {

class PlotterManager 
:public virtual Slash::Core::IManager 
,public virtual Slash::UI::IPlotterManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const;
  virtual void* cast(const std::string&) const;
public: //IPlotterManager
  virtual bool setArguments(const std::vector<std::string>&);
  virtual Slash::UI::IPlotter* create(const std::string&);
public:
  PlotterManager(Slash::Core::ISession&);
  virtual ~PlotterManager();
private:
  Slash::UI::IUI* loadUI();
private:
  std::string fName;
  Slash::Core::ISession& fSession;
  std::vector<std::string> fArgs;  
};

}

#endif


