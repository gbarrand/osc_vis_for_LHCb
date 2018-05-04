#ifndef Slash_UI_ICallbackData_h
#define Slash_UI_ICallbackData_h

#include <string>

namespace Slash {namespace UI {class IWidget;}}

namespace Slash {

namespace UI {

class ICallbackData {
public:
  virtual ~ICallbackData() {};
public:
  /*
  enum Event {
    ButtonPress = 0,
    ButtonRelease = 1,
    MotionNotify = 2
  };*/
  virtual IWidget* widget() const = 0;
  virtual std::string component() const = 0;
  virtual std::string value() const = 0;
  virtual std::string event() const = 0;
  virtual int x() const = 0;
  virtual int y() const = 0;
  virtual bool copy(const ICallbackData&) = 0;
};

} //UI

} //Slash

#endif
