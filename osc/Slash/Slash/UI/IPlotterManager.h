#ifndef Slash_UI_IPlotterManager_h
#define Slash_UI_IPlotterManager_h

#include <string>
#include <vector>

namespace Slash {namespace UI {class IPlotter;}}

namespace Slash {

namespace UI {

class IPlotterManager {
public:
  virtual ~IPlotterManager() {}
public:
  virtual bool setArguments(const std::vector<std::string>&) = 0;
  virtual IPlotter* create(const std::string&) = 0;
};

} //UI

} //Slash

#endif
