#ifndef Slash_UI_IUI_Manager_h
#define Slash_UI_IUI_Manager_h

#include <string>
#include <vector>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace UI {class IUI;}}

namespace Slash {

namespace UI {

class IUI_Manager {
public:
  virtual ~IUI_Manager() {};
public:
  typedef IUI*(*Factory)(Core::ISession&,
                         const std::vector<std::string>&);
public:
  virtual IUI* create(const std::string& driver,
                      const std::string& file,
                      const std::vector<std::string>& args,
                      bool show) = 0;
  virtual IUI* find(const std::string& name) const = 0;

  virtual bool add(IUI*) = 0;
};

} //UI

} //Slash

#endif
