#ifndef Slash_Data_IVisualizeAction_h
#define Slash_Data_IVisualizeAction_h

#include <string>
#include <vector>

namespace Slash {namespace UI {class IStyle;}}

namespace Slash {
namespace Data {

class IVisualizeAction {
public:
  virtual ~IVisualizeAction() {}
public:
  virtual bool execute(const std::vector<std::string>&,const Slash::UI::IStyle&) = 0;
};

}}

#endif



