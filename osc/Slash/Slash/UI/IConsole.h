#ifndef Slash_UI_IConsole_h
#define Slash_UI_IConsole_h

/**
 * @author G.Barrand
 */

#include <ostream>

namespace Slash {

namespace UI {

class IConsole {
public:
  virtual ~IConsole() {}
public: 
  virtual std::ostream& out() = 0;
  virtual bool beginOut() = 0;
  virtual bool endOut() = 0;
};

} //UI

} //Slash

#endif
