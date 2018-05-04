#ifndef OnXLab_Representer_h
#define OnXLab_Representer_h

#include <ostream>

namespace Slash {namespace Core {class IRelationManager;}}

namespace Slash {namespace UI {class IUI;}}
namespace Slash {namespace UI {class IWidget;}}

class SbPlottableObject;
class SoPlotter;

namespace AIDA {
  class IBaseHistogram;
  class IFunction;
  class IDataPointSet;
}

namespace OnXLab {

class Representer {
public:
  enum EmitMode {
    NONE,
    TOUCH,
    RENDER
  };
public:
  static SbPlottableObject* represent(const AIDA::IBaseHistogram&,Slash::Core::IRelationManager&,SoPlotter&,Slash::UI::IUI&,Slash::UI::IWidget&,std::ostream&,int verb,bool retain,EmitMode);
  static SbPlottableObject* represent(const AIDA::IFunction&,Slash::Core::IRelationManager&,SoPlotter&,Slash::UI::IUI&,Slash::UI::IWidget&,std::ostream&,int verb,bool retain,EmitMode);
  static SbPlottableObject* represent(const AIDA::IDataPointSet&,Slash::Core::IRelationManager&,SoPlotter&,Slash::UI::IUI&,Slash::UI::IWidget&,std::ostream&,int verb,bool retain,EmitMode);
};

}

#endif
