#ifndef Slash_UI_IWidgetFactory_h
#define Slash_UI_IWidgetFactory_h

#include <string>
#include <vector>

namespace Slash {namespace UI {class IWidget;}}
namespace Slash {namespace UI {class IWidgetClass;}}

namespace Slash {

namespace UI {

class IWidgetFactory {
public:
  virtual ~IWidgetFactory() {}
public: 
  virtual bool createComponent(const std::string& aName,
                               const std::string& aClass,
                               const std::string& aParent) = 0;

  virtual IWidget* createWidget(const IWidgetClass& aClass,
                                const std::string& aName,
                                IWidget& aParent,
                                const std::vector<std::string>& aArgs = std::vector<std::string>()) = 0;
};

} //UI

} //Slash

#endif
