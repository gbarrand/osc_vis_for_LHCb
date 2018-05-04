#ifndef OnX_UI_Manager_h
#define OnX_UI_Manager_h

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/IUI_Manager.h>

#include <list>

namespace OnX {

class UI_Manager
:public virtual Slash::Core::IManager 
,public virtual Slash::UI::IUI_Manager 
{
public: //Slash::Core::IManager
  virtual std::string name() const;
  virtual void* cast(const std::string&) const;
public: //Slash::UI::IUI_Manager
  virtual Slash::UI::IUI* create(const std::string& driver,
                      const std::string& file,
                      const std::vector<std::string>&,
                      bool show);
  virtual Slash::UI::IUI* find(const std::string&) const;
  virtual bool add(Slash::UI::IUI*);
public:
  UI_Manager(Slash::Core::ISession&);
  virtual ~UI_Manager();
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  Slash::UI::IUI* fUI;
};

}

#endif




