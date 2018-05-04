// this :
#include <OnX/Core/Widget.h>

#include <inlib/cast>

#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/IUI.h>

#include "../source/Core/Proxy.h"

OnX::Widget::Widget(
 Slash::UI::IUI& aUI
,inlib::xml::factory& aFactory
,inlib::xml::tree* aParent
,Slash::UI::IWidgetClass& aWidgetClass
)
:inlib::xml::tree("widget",aFactory,aParent)
,fUI(aUI) //FIXME
,fWidgetClass(aWidgetClass)
,fWidget(0)
,fProxy(0)
{}

OnX::Widget::~Widget() {
  //if(fUI.currentWidget()==this) fUI.setCurrentWidget(0); //FIXME
}

void* OnX::Widget::cast(inlib::cid a_class) const {
  return inlib::cmp_cast<inlib::xml::tree>(this,a_class); //by cid.
}

void* OnX::Widget::cast(const std::string& a_class) const {
  INLIB_IF_CAST(OnX::Widget)
  else INLIB_IF_CAST(Slash::UI::IWidget)
  else return inlib::xml::tree::cast(a_class); //by string.
}

std::string OnX::Widget::name() const { 
  std::string value;
  attribute_value("name",value);
  return value;
}

const Slash::UI::IWidgetClass& OnX::Widget::widgetClass() const {
  return fWidgetClass;
}

void* OnX::Widget::nativeWidget() const {return get_data1();}

void OnX::Widget::setProxy(void* aWidget,Proxy aProxy) { 
  fWidget = aWidget;
  fProxy = aProxy;
}

bool OnX::Widget::invalidate() {
  set_data(0,0,0);
  return true;
}

bool OnX::Widget::size(unsigned int& aW,unsigned int& aH) {
  if(!fProxy) return false;
  std::vector<std::string> args;
  void* ret;
  if(!fProxy(fWidget,PX_SIZE,args,ret)) return false;
  typedef unsigned int uint_t;
  uint_t* sz = (uint_t*)ret;
  aW = sz[0];
  aH = sz[1];
  delete [] sz;        
  return true;
}

bool OnX::Widget::position(int& aX,int& aY) {
  if(!fProxy) return false;
  std::vector<std::string> args;
  void* ret;
  if(!fProxy(fWidget,PX_POSITION,args,ret)) return false;
  int* pos = (int*)ret;
  aX = pos[0];
  aY = pos[1];
  delete [] pos;        
  return true;
}

bool OnX::Widget::write(
 const std::string& aFile
,const std::string& aFormat
,const std::string& aOptions
)
{
  if(!fProxy) return false;
  std::vector<std::string> args;
  args.push_back(aFile);
  args.push_back(aFormat);
  args.push_back(aOptions);
  void* ret;
  return fProxy(fWidget,PX_WRITE,args,ret);
}

bool OnX::Widget::manage(bool a_yes_no) {
  if(!fProxy) return false;
  std::vector<std::string> args;
  args.push_back(a_yes_no?"true":"false");
  void* ret;
  return fProxy(fWidget,PX_MANAGE,args,ret);
}

bool OnX::Widget::show(){
  if(!fProxy) return false;
  std::vector<std::string> args;
  void* ret;
  return fProxy(fWidget,PX_SHOW,args,ret);
}

bool OnX::Widget::hide(){
  if(!fProxy) return false;
  std::vector<std::string> args;
  void* ret;
  return fProxy(fWidget,PX_HIDE,args,ret);
}

bool OnX::Widget::refresh(){
  if(!fProxy) return false;
  std::vector<std::string> args;
  void* ret;
  return fProxy(fWidget,PX_REFRESH,args,ret);
}

bool OnX::Widget::setParameter(const std::string& aName,const std::string& aValue){
  //FIXME : avoid passing by IUI.
  return fUI.setParameter(name()+"."+aName,aValue);
}

bool OnX::Widget::parameterValue(const std::string& aName,std::string& aValue){
  //FIXME : avoid passing by IUI.
  return fUI.parameterValue(name()+"."+aName,aValue);
}

#include <OnX/Core/CallbackData.h>
bool OnX::Widget::executeCallbacks(const std::string& aCallback){
  std::string name;
  attribute_value("name",name);
  bool found = false;
  inlib::xml::looper _for(*this);
  while(inlib::xml::element* _elem = _for.next_element()) {
    if(aCallback==_elem->name()) {

      std::string sexec;
      _elem->attribute_value("exec",sexec);

      fUI.setCallbackData(CallbackData(this,name,"","",0,0));
      fUI.executeScript(sexec,_elem->value(),_elem->attributes()); //FIXME : ret val
      fUI.setCallbackData(CallbackData());

      found = true;
    }
  }
  return found;
}
