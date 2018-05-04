#ifndef OnX_Action_h
#define OnX_Action_h 

#include <vector>

#include <OnX/Core/CallbackData.h>

namespace Slash {namespace UI {class IUI;}}
namespace Slash {namespace UI {class IWidget;}}
class IIB;

namespace OnX {

class Action {
public:
  typedef void* NativeWidget;
  typedef std::pair<std::string,std::string> Parameter;
  typedef std::vector<Parameter> Parameters;
public:
  Action(Slash::UI::IUI&);
  Action(Slash::UI::IUI&,
         Slash::UI::IWidget*,
         NativeWidget,
         const std::string&,const std::string&,const Parameters&,
         const std::string&,int);
  Action(const Action&);
  Action& operator=(const Action&);
public:
  std::string name() const;
  bool execute();

  //bool equal(const Action&) const;
  virtual ~Action();
  bool isBackground() const;
  void setBackground(bool); 
  Slash::UI::IWidget* widget() const;
  NativeWidget nativeWidget() const;
  void setID(int);
  int getID() const;
  Slash::UI::IUI& ui() const;

  void setCallbackData(const CallbackData&);
  CallbackData callbackData() const;
  std::string script() const;
  std::string interpreter() const;
  Parameters parameters() const;


  void setInterpreter(const std::string&);
  void setScript(const std::string&);
private:
  Slash::UI::IUI& fUI;
  Slash::UI::IWidget* fWidget;
  NativeWidget fNativeWidget;
  std::string fName;
  int fID;
  std::string fInterpreter;
  std::string fScript;
  Parameters fParameters;
  CallbackData fCallbackData;
};

}

#endif


