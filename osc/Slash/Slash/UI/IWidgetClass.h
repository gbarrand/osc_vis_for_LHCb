#ifndef Slash_UI_IWidgetClass_h
#define Slash_UI_IWidgetClass_h

/**
 * Pure abstract interface for a widget class.
 *
 * 
 * @author L.Garnier
 */

#include <string>
#include <vector>

namespace Slash {

namespace UI {

class IWidgetClass {
public:
  virtual ~IWidgetClass() {}
public: 
  virtual std::string name() const = 0;
  virtual std::vector<std::string> attributes() const = 0;
  virtual std::vector<std::string> children() const = 0;
  virtual std::vector<std::string> properties() const = 0;
  virtual std::vector<std::string> callbacks() const = 0;
  virtual void setAttributes (const std::vector<std::string>&) = 0;
};

} //UI

} //Slash

#endif
