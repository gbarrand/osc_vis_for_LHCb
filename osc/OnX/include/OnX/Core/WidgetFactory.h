#ifndef OnX_WidgetFactory_h
#define OnX_WidgetFactory_h 

// Inheritance :
#include <inlib/xml/tree>

// Slash :
#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/IUI.h>

#include <inlib/cast>

//#include <OnX/Core/Widgets.h>

namespace OnX {

class WidgetFactory : public inlib::xml::factory {
public:
  WidgetFactory(Slash::UI::IUI& aUI,std::ostream& a_out,const std::vector<Slash::UI::IWidgetClass*>& aWidgetClasses)
  :inlib::xml::factory()
  ,fUI(aUI)
  ,f_out(a_out)
  ,fWidgetClasses(aWidgetClasses)
  {}
  virtual ~WidgetFactory(){}

public:
  //still in WidgetFactory.cxx

  virtual inlib::xml::tree* create(const std::string& aTagName,const std::vector<inlib::xml::tree::atb>& aAtts,inlib::xml::tree* aParent);
  
private:
  inline Slash::UI::IWidgetClass* findWidgetClass(const std::string& aName) const{
    unsigned int number = fWidgetClasses.size();
    for (unsigned int index=0;index<number;index++) {
      if (fWidgetClasses[index]->name()==aName) {
        return fWidgetClasses[index];
      } 
    }
    return 0;
  }

private:
  Slash::UI::IUI& fUI;
  std::ostream& f_out;
  const std::vector<Slash::UI::IWidgetClass*>& fWidgetClasses;
};

}

#endif
