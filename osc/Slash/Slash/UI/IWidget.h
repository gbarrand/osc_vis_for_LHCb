#ifndef Slash_UI_IWidget_h
#define Slash_UI_IWidget_h

/**
 * Pure abstract interface for a widget.
 *
 * 
 * @author G.Barrand
 */

#include <string>

namespace Slash {namespace UI {class IWidgetClass;}}

namespace Slash {

namespace UI {

class IWidget {
public:
  virtual ~IWidget() {}
  virtual void* cast(const std::string& className) const = 0;
public: 
  virtual const IWidgetClass& widgetClass() const = 0;
  virtual std::string name() const = 0;
  virtual void* nativeWidget() const = 0;
  virtual bool executeCallbacks(const std::string& callbackName) = 0;
  virtual bool size(unsigned int& width,unsigned int& height) = 0;
  virtual bool position(int& x,int& y) = 0;

  virtual bool write(const std::string& file,
                     const std::string& format,
                     const std::string& options = "") = 0;

  virtual bool setParameter(const std::string&,const std::string&) = 0;
  virtual bool parameterValue(const std::string&,std::string&) = 0;
  virtual bool refresh() = 0;

  virtual bool manage(bool) = 0;
  virtual bool show() = 0;
  virtual bool hide() = 0;
};

} //UI

} //Slash

#endif
