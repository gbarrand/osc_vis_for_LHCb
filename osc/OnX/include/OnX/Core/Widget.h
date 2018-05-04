#ifndef OnX_Widget_h
#define OnX_Widget_h 

// Inheritance :
#include <Slash/UI/IWidget.h>
#include <inlib/xml/tree>

namespace Slash {namespace UI {class IWidgetClass;}}
namespace Slash {namespace UI {class IUI;}}

namespace OnX {

class Widget : public virtual Slash::UI::IWidget, public inlib::xml::tree {
public: //Slash::UI::IWidget
  virtual void* cast(const std::string& aTo) const;
  virtual const Slash::UI::IWidgetClass& widgetClass() const;
  virtual std::string name() const;
  virtual void* nativeWidget() const;
  virtual bool executeCallbacks(const std::string&);
  virtual bool size(unsigned int&,unsigned int&);
  virtual bool position(int&,int&);
  virtual bool write(const std::string& file,
                     const std::string& format,
                     const std::string& options = "");
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&);
  virtual bool manage(bool);
  virtual bool refresh();
  virtual bool show();
  virtual bool hide();
public: //inlib::xml::tree
  virtual void* cast(inlib::cid) const;
  virtual bool invalidate();
public:
  Widget(Slash::UI::IUI&,
         inlib::xml::factory&,
         inlib::xml::tree*,
         Slash::UI::IWidgetClass&);
  virtual ~Widget();
public:
  typedef bool(*Proxy)(void*,
                       const std::string&,const std::vector<std::string>&,
                       void*&);
  void setProxy(void* aViewer,Proxy aProxy);
protected:
  Slash::UI::IUI& fUI;
  Slash::UI::IWidgetClass& fWidgetClass;
  void* fWidget; //native widget.
  Proxy fProxy;
};

}

#endif
