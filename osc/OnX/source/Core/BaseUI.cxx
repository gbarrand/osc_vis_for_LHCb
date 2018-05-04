// this :
#include <OnX/Core/BaseUI.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Core/IRelation.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/IWidgetFactory.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/FileGetter.h>

#include <Slash/Net/protocol>

#include <inlib/sys/file>
#include <inlib/sprintf>
#include <inlib/file>
#include <inlib/system>
#include <inlib/args>
#include <inlib/cast>
#include <exlib/xml/parser>

#include "inlib"

#include <fstream>

// Lib :
#include <Lib/Messages.h>
#include <Lib/Debug.h>

// OnX :
#include <OnX/Version.h>
#include <OnX/Core/CallbackData.h>
#include <OnX/Core/Messages.h>
#include <OnX/Core/WidgetClass.h>
#include <OnX/Core/Widget.h>
#include <OnX/Core/WidgetFactory.h>

#include <algorithm> //std::find

#include <cstdio> //::remove

namespace OnX {

class UIML : public exlib::xml::parser {
public:
  UIML(WidgetFactory& aFactory,std::ostream& a_out,int aVerbose)
  : exlib::xml::parser(aFactory,a_out,aVerbose){
    std::vector<std::string> tags(5);
    tags[0] = "OnX";
    tags[1] = "widget";
    tags[2] = "reference";
    tags[3] = "comment";
    tags[4] = "aliases";
    set_tags(tags);
  }
};

class ExecuteMessage : public virtual Slash::Core::IMessage {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnX::ExecuteMessage)
    else INLIB_IF_CAST(Slash::Core::IMessage)
    else return 0;
  }
  virtual std::string message() const {
    return std::string("OnX::ExecuteMessage : \"") 
           + fAction->name() + std::string("\".");}
public:
  ExecuteMessage(OnX::Action* aAction):fAction(aAction) { 
    Lib::Debug::increment("OnX::ExecuteMessage"); 
  }
  virtual ~ExecuteMessage() {
    Lib::Debug::decrement("OnX::ExecuteMessage"); 
  }
  OnX::Action* action() const {return fAction;}
private:
  OnX::Action* fAction; //we may have a copy if needed.
};

}

static bool saveItemML(inlib::xml::tree&,const std::string&,int,std::ostream&);
static bool saveItemML(inlib::xml::tree&,std::ostream&,int,int,std::ostream&);
static bool isGeneratedName(const std::string&);


OnX::Execute::Execute(BaseUI& aUI,const std::string& aWhat)
:fUI(aUI),fWhat(aWhat){}
const std::string& OnX::Execute::what() const {return fWhat;}
OnX::BaseUI& OnX::Execute::baseUI(){return fUI;}

#ifdef WIN32
// disable the warning about the usage of "this" in the constructor.
#pragma warning(disable:4355)
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnX::BaseUI::BaseUI(Slash::Core::ISession& aSession)
:fSession(aSession)
,fTopItemML(0)
,fScriptManager(0)
,fNetManager(0)
,fFileGetter(0)
,fNetCyclic(0)
,fWidgetNameCounter(0)
,fBeingDestroyed(false)
,fVerboseLevel(0)
,fSteerLevel(0)
,fRelationManager(0)
,fCurrentWidget(0)
,fAskToTerminal(false)
//IB
,fWidgetFactory(*this,fSession.cout(),fWidgetClasses)
{
  Lib::Debug::increment("OnX::BaseUI");

  fScriptManager = Slash::scriptManager(fSession);
  fRelationManager = Slash::relationManager(fSession);
  fNetManager = Slash::netManager(fSession);
  if(fNetManager) {
    fFileGetter = new Slash::FileGetter(fSession.cout(),fNetManager->socket());
  }

  std::string svalue;
  if(fSession.parameterValue("UI_Manager.verbose",svalue)) {
    int value;
    if(inlib::to<int>(svalue,value)) fVerboseLevel = value;
  } else {
    fVerboseLevel = fSession.verboseLevel();
  }

  // IB :
  createWidgetTable();
  createLanguageList();
  createPropertyList();
}

OnX::BaseUI::BaseUI(const BaseUI& aFrom)
:fSession(aFrom.fSession)
,fTopItemML(0)
,fScriptManager(0)
,fNetManager(0)
,fFileGetter(0)
,fNetCyclic(0)
,fWidgetNameCounter(0)
,fBeingDestroyed(false)
,fVerboseLevel(0)
,fSteerLevel(0)
,fRelationManager(0)
,fCurrentWidget(0)
,fAskToTerminal(false)
//IB
,fWidgetFactory(*this,aFrom.fSession.cout(),fWidgetClasses)
{
  Lib::Debug::increment("OnX::BaseUI");
}

OnX::BaseUI::~BaseUI() { 
  //FIXME : Python : on a Python callback, someone
  //                 may do a sys.exit(0) that will
  //                 delete the ui object whilst in ui.steer() !
  //if(fSteerLevel) {
  //}

  fCurrentWidget = 0;

  // Remove relations concerning this UI :
  if(fRelationManager) fRelationManager->removeWith(this);

 {for(unsigned int index=0;index<fFactories.size();index++) {
    delete fFactories[index];
  }
  fFactories.clear();}

  // IB :
 {for(unsigned int index=0;index<fWidgetClasses.size();index++) {
    delete fWidgetClasses[index];
  }
  fWidgetClasses.clear();}

 {for(std::list<Action*>::iterator it=fActions.begin();it!=fActions.end();) {
    Action* action = *it;
    it = fActions.erase(it);
    delete action;
  }}

  delete fFileGetter;
  delete fNetCyclic;

  delete fTopItemML;
  Lib::Debug::decrement("OnX::BaseUI");
}

OnX::BaseUI& OnX::BaseUI::operator=(const BaseUI& aFrom){ return *this;}

int OnX::BaseUI::steerLevel() const {return fSteerLevel;}

int OnX::BaseUI::steer(){
  if(!fTopItemML) return 0;
  executeCallbacks(*fTopItemML,"enterSteering");
  if(fNetCyclic) fNetCyclic->start();
  int value = nativeSteer();
  if(fNetCyclic) fNetCyclic->stop();
  executeCallbacks(*fTopItemML,"leaveSteering");
  return value;
}

static bool dld_dummy_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*){
  //::printf("debug : dummy\n");
  return true;
}

bool OnX::BaseUI::finalize(){
  if(!fTopItemML) return true;

  // Disbale terminal here (and not in <Xxx>UI)
  // and then before the destroy callbacks in order 
  // to avoid : DLD> OnX_Finalize.
  disableTerminal(); 

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::finalize :"
        << " execute destroyCallbacks..." << std::endl;
  }

  //NOTE : at this point the UI_Manager is no more in the session
  //       and then a Slash::find_UI will fail, and then
  //       the DLD destroy callbacks will fail.
 {Slash::UI::IInterpreter* interpreter = 
    fScriptManager->findInterpreter("DLD");
  if(interpreter) {
    interpreter->setFunction(dld_dummy_Function,0);
  }}

  Execute tag(*this,"destroy");
  fTopItemML->post_execute_backward(executeItemMLCallbacks,&tag);

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::finalize :"
        << " execute destroyCallbacks : end" << std::endl;
  }

  return true;
}

bool OnX::BaseUI::beingDestroyed() const {return fBeingDestroyed;}

Slash::Core::ISession& OnX::BaseUI::session(){return fSession;}

std::ostream& OnX::BaseUI::out() const {return fSession.cout();}

bool OnX::BaseUI::isValid() const {
  if(!fTopItemML) return false;
  return true;
}

void* OnX::BaseUI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(Slash::UI::IUI)
  else INLIB_IF_CAST(Slash::UI::IIB)
  else INLIB_IF_CAST(OnX::BaseUI)
  else return 0;
}

//NOTE : keyword use too in Action.cxx

#define S_BACK "background"

#include <inlib/net/base_socket>

bool OnX::BaseUI::executeScript(
 const std::string& aInterp
,const std::string& aScript
,const Slash::UI::IUI::Options& aParams
){
  if(!fScriptManager) return false;

  //{for(unsigned int index=0;index<aParams.size();index++) {
  //   ::printf("debug : BaseUI : %d : %s %s\n",index,
  //      aParams[index].first.c_str(),aParams[index].second.c_str());
  //}}

  inlib::args params(aParams);

  std::string where = "local";
  if(!params.find("where",where)) where = "local";

  bool bg = false;
  if(params.find(S_BACK,bg)) params.remove(S_BACK);

  //old syntax :
  std::string interp = aInterp;
  std::vector<std::string> words;
  inlib::words(aInterp," ",false,words);
  if(words.size()==1) {
  } else if(words.size()==2) {
    if(((words[1]=="bg")||(words[1]==S_BACK)) ){
      //FIXME : correct Panoramix and then let the below message.
      //std::ostream& out = fSession.cout();
      //out << "OnX::BaseUI::executeScript :"
      //    << " deprecated bg syntax."
      //    << " Use the background=\"yes\" XML attribute."
      //    << std::endl;
      bg = true;
      interp = words[0];
    } else {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::executeScript :"
          << " bad syntax for interp " << inlib::sout(aInterp)
          << std::endl;
      return false;
    }
  } else {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::executeScript :"
        << " bad syntax for interp " << inlib::sout(aInterp)
        << std::endl;
    return false;
  }

  if(bg) {
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::executeScript :"
          << " post execution of "
          << " " << inlib::sout(interp)
          << " " << inlib::sout(aScript)
          << std::endl;
    }
    // Process in background :
    // Have a copy of fCallbackData in Action.

    Action* action = 
      new Action(*this,0,0,interp,aScript,params.get_args(),"post",0);
    action->setCallbackData(fCallbackData);
    notify(new OnX::ExecuteMessage(action)); //ExecuteMessage takes ownershup.
    return true; 
  } 

  if(where=="remote"){
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::executeScript :"
          << " remote execution of "
          << " " << inlib::sout(interp)
          << " " << inlib::sout(aScript)
          << std::endl;
    }

    if(!fNetManager) {
      return fScriptManager->executeScript(interp,aScript,aParams);
    }

    //FIXME : pass fCallbackData
    //FIXME : pass aParams
    inlib::net::base_socket& socket = fNetManager->socket();
    if(!socket.send_string(inlib_net_protocol_cbk)) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::executeScript :"
          << " send_string failed."
          << std::endl;
      //socket.disconnet();
      return false;
    }
    if(!socket.send_string(interp+" "+aScript)) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::executeScript :"
          << " send_string failed."
          << std::endl;
      //socket.disconnet();
      return false;
    }
    //FIXME : send params.
    return true;

  } else if(where=="local"){

    return fScriptManager->executeScript(interp,aScript,aParams);

  } else {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::executeScript :"
        << " bad where keyword " << inlib::sout(where)
        << " for script " << inlib::sout(aScript)
        << std::endl;
    return false;
  }
}

void OnX::BaseUI::echo(const std::string& aString){
  std::ostream& out = fSession.cout();
  out << aString << std::endl;
}

void OnX::BaseUI::addDispatcher(Slash::UI::IUI::Dispatcher aDispatcher){
  if(std::find(fDispatchers.begin(),
               fDispatchers.end(),
               aDispatcher)!=fDispatchers.end()) return;
  fDispatchers.push_back(aDispatcher);
}

void OnX::BaseUI::removeDispatcher(Slash::UI::IUI::Dispatcher aDispatcher){
  fDispatchers.remove(aDispatcher);
}

std::string OnX::BaseUI::callbackValue(){return fCallbackData.value();}
const Slash::UI::ICallbackData& OnX::BaseUI::callbackData(){
  return fCallbackData;
}

bool OnX::BaseUI::setCallbackData(const Slash::UI::ICallbackData& aFrom){
  return fCallbackData.copy(aFrom);
}

void OnX::BaseUI::setCallbackData(const CallbackData& aValue){
  fCallbackData = aValue;
}

void OnX::BaseUI::deleteChildren(const std::string&){}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX::BaseUI::addAction(Action* aAction){fActions.push_back(aAction);}

void OnX::BaseUI::removeActions(OnX::Action::NativeWidget aComponent){
  for(std::list<Action*>::iterator it=fActions.begin();it!=fActions.end();) {
    Action* action = *it;
    if(action->nativeWidget()==aComponent) {
      it = fActions.erase(it);
      delete action;
    } else {
      ++it;
    }
  }
}

OnX::Action* OnX::BaseUI::getAction(OnX::Action::NativeWidget aComponent,const std::string& aName,int aIndex){
  int index = 0;
  for(std::list<Action*>::iterator it=fActions.begin();
      it!=fActions.end();++it) {
    Action* action = *it;
    if( (action->nativeWidget()==aComponent) &&
        (action->name()==aName) ) {
      if(index==aIndex) {
        return action;
      } else {
        index++;
      }
    }
  }
  return 0;
}

std::vector<std::string> OnX::BaseUI::disableCallbackScript(OnX::Action::NativeWidget aComponent,const std::string& aName){
  // Disable the scripts of the aName callback.
  // It can be used to avoid recursive loop in case
  // the same callback is indirectly triggered in the scripts.
  // Usage :
  //   std::vector<std::string> tmp = disableCallbackScript(...)
  //   // call something which does a callback
  //   enableCallbackScript(...,tmp)
  std::vector<std::string> scripts;
  for(std::list<Action*>::iterator it=fActions.begin();
      it!=fActions.end();++it) {
    Action* action = *it;
    if( (action->nativeWidget()==aComponent) &&
        (action->name()==aName) ) {
      scripts.push_back(action->script());
      action->setScript("");
    }
  }
  return scripts;
}

bool OnX::BaseUI::enableCallbackScript(OnX::Action::NativeWidget aComponent,const std::string& aName,const std::vector<std::string>& aScripts){
  unsigned int index = 0;
  for(std::list<Action*>::iterator it=fActions.begin();
      it!=fActions.end();++it) {
    Action* action = *it;
    if( (action->nativeWidget()==aComponent) &&
        (action->name()==aName) ) {
      if(index>=aScripts.size()) return false;
      action->setScript(aScripts[index]);
      index++;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::load(Slash::UI::IWidget& aParent,const std::string& aToLoad,bool aIsFile,bool aShow){
  // Load a file or a string and add its items in the fTopItemML tree.
  inlib::xml::tree* itemML = INLIB_CAST(aParent,inlib::xml::tree);
  if(!itemML) {
    out() << "OnX::BaseUI::load :"
          << " IWidget parent is not a inlib::xml::tree."
          << std::endl;
    return false;
  }
  inlib::xml::tree* top;
  return load(itemML,aToLoad,aIsFile,aShow,true,top);
}

bool OnX::BaseUI::load(const std::string& aParent,const std::string& aToLoad,bool aIsFile,bool aShow) {
  // Load a file or a string and add its items in the fTopItemML tree.
  if(fTopItemML) {
    inlib::xml::tree* itemML = 0;
    if(aParent.empty()) {
      itemML = fTopItemML;
    } else {
      itemML = fTopItemML->find_item(aParent);
    }
    if(!itemML) {
      out() << "OnX::BaseUI::load :"
            << " parent widget " << inlib::sout(aParent) << " not found."
            << std::endl;
      return false;
    }
    inlib::xml::tree* top;
    if(!load(itemML,aToLoad,aIsFile,aShow,true,top)) return false;
  } else {
    if(aParent.size()) {
      out() << "OnX::BaseUI::load :"
            << " can't specify a parent whilst no top."
            << std::endl;
      return false;
    } else {
      //startup (from UI_Manager::create).
      if(aToLoad=="@empty@") { //See OnX/examples/Python/gui.py
        // Someone may want to start a GUI from scratch
        // for example from Python. 
        // (See OnX/examples/Python/OnX_exa_gui_py.py)
        fTopItemML = new inlib::xml::tree("OnX",fWidgetFactory,0);
      } else {
        inlib::xml::tree* top;
        if(!load(0,aToLoad,aIsFile,aShow,false,top)) return false;
        fTopItemML = top;
        Execute tag(*this,"create");
        top->post_execute(executeItemMLCallbacks,&tag);
      }
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::save(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return false;
  return saveItemML(*fTopItemML,fTopItemML->file(),
                    fVerboseLevel,fSession.cout());
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::toWidgetAndResource(
 const std::string& aString
,std::string& aWidget
,std::string& aResource
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aWidget = "";
  aResource = "";
  std::vector<std::string> words;
  inlib::words(aString,".",false,words);
  if(words.size()!=2) {
    out() << "OnX::BaseUI::toWidgetAndResource :"
          << " Bad syntax in " << inlib::sout(aString) << "."
          << " <widget>.<parameter> expected." << std::endl;
    return false;
  }
  if( (words[0]=="currentViewer") || (words[0]=="viewer") ) {
    out() << "OnX::BaseUI::toWidgetAndResource :"
          << " words currentViewer and viewer are deprecated."
          << " Use ui_SoViewer helper." << std::endl;
    //FIXME : deprecate that. No reason that BaseUI contains
    //        any viewer logic.
    //aWidget = currentViewer();  
    //if(aWidget.empty()) return false; //No viewer around.
    return false;
  } else if(words[0]=="this") {
    out() << "OnX::BaseUI::toWidgetAndResource :"
          << " words this is deprecated."
          << " Use @this@ instead." << std::endl;
    aWidget = fCallbackData.component();
  } else {
    aWidget = words[0];
  }
  aResource = words[1];
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::element_value(
 const std::string& aWidget
,const std::string& aResource
,std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  if(!fTopItemML) return false;
  inlib::xml::tree* itemML = fTopItemML->find_item(aWidget);
  if(!itemML) return false;
  itemML->element_value(aResource,aValue);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Slash::UI::IWidget* OnX::BaseUI::findWidget(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return 0;
  inlib::xml::tree* itemML = fTopItemML->find_item(aName);
  if(!itemML) return 0;
  return INLIB_CAST(*itemML,Slash::UI::IWidget);
}
//////////////////////////////////////////////////////////////////////////////
Slash::UI::IWidget* OnX::BaseUI::findWidgetByAttribute(
 const std::string& aAttribute
,const std::string& aValue
,bool aUpDown //true
,bool aLeftRight //true
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return 0;
  inlib::xml::tree* itemML = 
    fTopItemML->find_by_attribute(aAttribute,aValue,aUpDown,aLeftRight);
  if(!itemML) return 0;
  return INLIB_CAST(*itemML,Slash::UI::IWidget); //May be not an IWidget !
}

void* OnX::BaseUI::findNativeWidget(const std::string& aName){
  if(!fTopItemML) return 0;
  inlib::xml::tree* itemML = fTopItemML->find_item(aName);
  if(!itemML) return 0;
  return itemML->get_data1();
}

inlib::xml::tree* OnX::BaseUI::findItemML(const std::string& aName) {
  if(!fTopItemML) return 0;
  return fTopItemML->find_item(aName);
}

bool OnX::BaseUI::isComponent(const std::string& aName){
  return (findNativeWidget(aName) ? true : false);
}

bool OnX::BaseUI::createNativeWidget(inlib::xml::tree& aItemML){
  if(!createIf(aItemML)) return true;

  const std::string& tagName = aItemML.tag_name();

  if(tagName=="comment") {

    return true;

  } else if(tagName=="aliases") {

    return true;

  } else if( (tagName=="OnX") ||
             (tagName=="reference") ) {

    return createChildren(aItemML);

  } else if(tagName=="widget") {

    std::string sclass;
    if(!aItemML.attribute_value("class",sclass)) {
      std::ostream& out = fSession.cout();
      out << "BaseUI::createNativeWidget :" 
          << " widget without a class." << std::endl;
      aItemML.dump_xml(fSession.cout(),"  ");
      return false;
    }

    if(sclass=="InputDialog") {
      return createChildren(aItemML);
    }
    
    std::string name = buildName(aItemML);
    inlib::xml::tree* parent = parentItemML(aItemML); 

    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "BaseUI::createNativeWidget :" 
          << " class " << inlib::sout(sclass)
          << " name " << inlib::sout(name)
          << "..." << std::endl;
    }

    return createNativeWidget(aItemML,sclass,name,parent);

  } else {

    std::ostream& out = fSession.cout();
    out << "BaseUI::createNativeWidget :"
        << " unknown XML tag " << inlib::sout(tagName) 
        << std::endl;
    return false;

  }
}

Slash::UI::IWidget* OnX::BaseUI::createWidget(
 const Slash::UI::IWidgetClass& aClass
,const std::string& aName
,Slash::UI::IWidget& aParent
,const std::vector<std::string>& aArgs
){
  // Look for a plugin factory :
  for(unsigned int index=0;index<fFactories.size();index++) {
    Slash::UI::IWidgetFactory* fac = fFactories[index];
    Slash::UI::IWidget* w = fac->createWidget(aClass,aName,aParent,aArgs);
    if(w) return w;
  }
  // Look to the predefined widgets :
  if(!fTopItemML) {
    out() << "OnX::BaseUI::createWidget :"
          << " topItemML not created." << std::endl;
    return 0;
  }
  inlib::xml::tree* parentItemML = INLIB_CAST(aParent,inlib::xml::tree);
  if(!parentItemML) {
    out() << "OnX::BaseUI::createWidget :"
          << " IWidget parent is not a inlib::xml::tree."
          << std::endl;
    return 0;
  }
  inlib::xml::tree* itemML = createItemML(aName,aClass.name(),parentItemML);
  if(!itemML) return 0;
  if(!createNativeWidget(*itemML)) return 0;
  Slash::UI::IWidget* w = INLIB_CAST(*itemML,Slash::UI::IWidget);
  if(!w) {
    out() << "OnX::BaseUI::createWidget :"
          << " inlib::xml::tree not a IWidget."
          << std::endl;
    return 0;
  }
  Execute tag(*this,"create");
  itemML->post_execute(executeItemMLCallbacks,&tag);
  return w;
}

void OnX::BaseUI::showShells(inlib::xml::tree& aItemML){
  // Show the shell widgets.
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  if(widget && widget->nativeWidget()) {
    std::string sshow;
    // If show property not here, we show the widget.
    // If show is here and is not "no", we show the widget.
    if( !aItemML.element_value("show",sshow) || (sshow!="no") ) {
      widget->show();
    }
  } else { // Look children :
    std::list<inlib::xml::tree*> list;
    aItemML.sub_trees(list);
    std::list<inlib::xml::tree*>::const_iterator it;
    for(it=list.begin();it!=list.end();++it) showShells(*(*it));
  }
}

bool OnX::BaseUI::show(){
  if(!fTopItemML) return false;
  showShells(*fTopItemML);
  return true;
}

bool OnX::BaseUI::show(const std::string& aName){
  Slash::UI::IWidget* w = findWidget(aName);
  if(!w) return false;
  w->show();
  return true;
}

bool OnX::BaseUI::createComponent(
 const std::string& aName
,const std::string& aClass
,const std::string& aParent
){
  // Look for a plugin factory :
  for(unsigned int index=0;index<fFactories.size();index++) {
    Slash::UI::IWidgetFactory* fac = fFactories[index];
    if(fac->createComponent(aName,aClass,aParent)) return true;
  }
  // Look to the predefined widgets :
  if(!fTopItemML) {
    out() << "OnX::BaseUI::createComponent :"
          << " topItemML not created." << std::endl;
    return false;
  }
  inlib::xml::tree* parentItemML = 0;
  if(aParent.size()) {
    parentItemML = fTopItemML->find_item(aParent);
    if(!parentItemML) {
      out() << "OnX::BaseUI::createComponent :"
            << " component " << inlib::sout(aParent) << " not found."
            << std::endl;
      return false;
    }
  } else {
    parentItemML = fTopItemML;
  }
  inlib::xml::tree* itemML = createItemML(aName,aClass,parentItemML);
  if(!itemML) return false;
  if(!createNativeWidget(*itemML)) return false;
  Execute tag(*this,"create");
  itemML->post_execute(executeItemMLCallbacks,&tag);
  return true;
}

bool OnX::BaseUI::destroyWidget(Slash::UI::IWidget*& aWidget){
  inlib::xml::tree* itemML = INLIB_CAST(*aWidget,inlib::xml::tree);
  if(!itemML) return false;
  bool status = destroyItemML(itemML);
  aWidget = 0;
  return status;
}

bool OnX::BaseUI::destroyComponent(const std::string& aName){
  if(!fTopItemML) return false;
  inlib::xml::tree* itemML = fTopItemML->find_item(aName);
  if(!itemML) return false;
  return destroyItemML(itemML);
}

bool OnX::BaseUI::copyComponent(
 const std::string& aName
,const std::string& aComponent
,const std::string& aParent
){
  if(!fTopItemML) {
    out() << "OnX::BaseUI::copyComponent :"
          << " topItemML not created." << std::endl;
    return false;
  }
  inlib::xml::tree* fromItemML = fTopItemML->find_item(aComponent);
  if(!fromItemML) {
    out() << "OnX::BaseUI::copyComponent :"
          << " component " << inlib::sout(aComponent) << " not found."
          << std::endl;
    return false;
  }
  inlib::xml::tree* parentItemML = 0;
  if(aParent.empty()) {
    parentItemML = fTopItemML;
  } else {
    parentItemML = fTopItemML->find_item(aParent);
    if(!parentItemML) {
      out() << "OnX::BaseUI::copyComponent :"
            << " parent component " << inlib::sout(aParent) << " not found." 
            << std::endl;
      return false;
    }
  }
  inlib::xml::tree* itemML = fromItemML->create_copy(parentItemML);
  if(!itemML) return false;
  parentItemML->add_child(itemML);
  itemML->set_attribute_value("name",aName);
  if(!createNativeWidget(*itemML)) return false;
  Execute tag(*this,"create");
  itemML->post_execute(executeItemMLCallbacks,&tag);
  return true;
}

inlib::xml::tree* OnX::BaseUI::parentItemML(inlib::xml::tree& aItemML){
  inlib::xml::tree* parent = aItemML.parent(); 
  while(parent) {
    void* tkParent = parent->get_data1();
    if(tkParent) return parent;
    parent = parent->parent();
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OnX::BaseUI::findChildren(
 const std::string& aComponent
,const std::string& aWhat
,const std::string& aParameter
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> found;
  if(!fTopItemML) return found;

  inlib::xml::tree* itemML = 0;
  if(aComponent.empty()) {
    itemML = fTopItemML;
  } else {
    itemML = fTopItemML->find_item(aComponent);
  }
  if(!itemML) return found;

  std::list<inlib::xml::tree*> list;
  itemML->sub_trees(list);

  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(searchChild(*(*it),aWhat,aParameter,found)) break;
  }

  return found;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::searchChild(
 inlib::xml::tree& aItemML
,const std::string& aWhat
,const std::string& aParameter
,std::vector<std::string>& aFound
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aWhat=="class") {
    // aParameter not used.
    std::string sclass;
    aItemML.attribute_value("class",sclass);
    aFound.push_back(sclass);
  } else if(aWhat=="name") {
    // aParameter not used.
    std::string value;
    aItemML.attribute_value("name",value);
    aFound.push_back(value);
  } else if(aWhat=="nameFromClass") {
    std::string sclass;
    aItemML.attribute_value("class",sclass);
    if(sclass==aParameter) {
      std::string value;
      aItemML.attribute_value("name",value);
      if(value.size()) {
        aFound.push_back(value);
      } else {
        out() << "OnX::BaseUI::searchChild :" 
              << " found a component with matching class "
              << inlib::sout(sclass)
              << " but without name." << std::endl;
      }
    }
  } else if(aWhat=="labelFromClass") {
    std::string sclass;
    aItemML.attribute_value("class",sclass);
    if(sclass==aParameter) {
      std::string value;
      aItemML.element_value("label",value);
      if(value.size()) {
        aFound.push_back(value);
      } else {
        out() << "OnX::BaseUI::searchChild :" 
              << " found a component with matching class "
              << inlib::sout(sclass)
              << " but without label." << std::endl;
      }
    }
  } else if(aWhat=="nameFromLabel") {
    std::string label;
    aItemML.element_value("label",label);
    if(label==aParameter) {
      std::string value;
      aItemML.attribute_value("name",value);
      if(value.size()) {
        aFound.push_back(value);
        return true; //stop.
      } else {
        out() << "OnX::BaseUI::searchChild :" 
              << " found a component with matching label "
              << inlib::sout(label)
              << " but without name." << std::endl;
      }
    }
  }

  std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(searchChild(*(*it),aWhat,aParameter,aFound)) return true;
  }

  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<std::string> OnX::BaseUI::findParent(
 const std::string& aComponent
,const std::string& aWhat
,const std::string& aParameter
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> found;
  if(!fTopItemML) return found;
  inlib::xml::tree* itemML = fTopItemML->find_item(aComponent);
  if(!itemML) return found;
  searchParent(*itemML,aWhat,aParameter,found);
  return found;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::searchParent(
 inlib::xml::tree& aItemML
,const std::string& aWhat
,const std::string& aParameter
,std::vector<std::string>& aFound
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  inlib::xml::tree* parent = aItemML.parent(); 
  while(parent) {

    if(aWhat=="name") {

      std::string name;
      parent->attribute_value("name",name);
      aFound.push_back(name);
      return true;

    } else if(aWhat=="class") {

      std::string sclass;
      if(!parent->attribute_value("class",sclass)){
        out() << "BaseUI::searchParent :" 
              << " ItemML without a class." << std::endl;
        return false;
      }
      aFound.push_back(sclass);
      return true;

    } else if(aWhat=="shell") {
      // aParameter not used.

      if(parent->tag_name()=="widget") {
        std::string sclass;
        if(!parent->attribute_value("class",sclass)){
          out() << "BaseUI::searchParent :" 
                << " ItemML without a class." << std::endl;
          return false;
        }
        if((sclass=="Window")||(sclass=="Dialog")) {
          std::string name;
          parent->attribute_value("name",name);
          aFound.push_back(name);
          return true;
        }
      }

    }
 
    parent = parent->parent();
  }

  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::executeCallbacks(
 const std::string& aComponent
,const std::string& aCallback
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return false;
  if(aComponent=="*") {
    Execute tag(*this,aCallback);
    fTopItemML->post_execute(executeItemMLCallbacks,&tag);
    return true;
  } else {
    inlib::xml::tree* itemML = fTopItemML->find_item(aComponent);
    if(!itemML) return false;
    return executeCallbacks(*itemML,aCallback);
  }
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::executeCallbacks(
 inlib::xml::tree& aItemML
,const std::string& aCallback
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  if(widget) {
    return widget->executeCallbacks(aCallback);
  } else {
    // Callback of an ItemML which is not a IWidget. //FIXME.
    //FIXME : the below duplicate OnX::Widget::executeCallbacks code.
    std::string name;
    aItemML.attribute_value("name",name);
    bool found = false;
    std::list<inlib::xml::element*> props;
    aItemML.sub_elems(props);
    std::list<inlib::xml::element*>::const_iterator it;
    for(it=props.begin();it!=props.end();++it) {
      if(aCallback==(*it)->name()) {
  
        std::string sexec;
        (*it)->attribute_value("exec",sexec);
  
        fCallbackData = CallbackData(0,name,"","",0,0);
        executeScript(sexec,(*it)->value(),(*it)->attributes()); //FIXME : ret val
        fCallbackData = CallbackData();
  
        found = true;
      }
    }
    return found;
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::destroyItemML(
 inlib::xml::tree*& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Execute tag(*this,"destroy");
  aItemML->post_execute_backward(executeItemMLCallbacks,&tag);

  /*FIXME
  std::string sclass;
  aItemML->attribute_value("class",sclass);
  if(sclass=="OnX") {
  } else if(sclass=="comment") {
  } else if(sclass=="aliases") {
  } else if(sclass=="Reference") {    
    // Do it with a better implementation of OnX::Widget.
  } else if(sclass=="InputDialog") {
  }*/

  // Remove in parent before calling destroyNativeWidget :
  inlib::xml::tree* parentItemML = aItemML->parent();
  if(parentItemML) {
    parentItemML->remove_child(aItemML,false); //false to not delete aItemML.
  } else { 
    std::string name;
    aItemML->attribute_value("name",name);
    out() << "OnX::BaseUI::destroyItemML :"
          << " WARNING : itemML " << inlib::sout(name) << " with no parent !" 
          << std::endl;
  }

  //Destroy native widgets.
  bool status = destroyNativeWidget(*aItemML);

  delete aItemML;
  aItemML = 0;
  return status;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool OnX::BaseUI::load(
 inlib::xml::tree* aParent // !aParent = startup
,const std::string& aToLoad
,bool aIsFile
,bool aShow
,bool aExecCreate
,inlib::xml::tree*& aTop
){
  aTop = 0;
  // Load a file or a string and add its items in the fTopItemML tree.
  UIML uiml(fWidgetFactory,out(),fVerboseLevel);
  if(fNetManager) {
    uiml.set_file_getter(fFileGetter);
  } else {
    uiml.set_file_getter(0);
  }
  if(aIsFile) {
    if(!uiml.load_file(aToLoad,false)) return false;
  } else {
    if(!uiml.load_string(aToLoad)) return false;
  }
  inlib::xml::tree* top = uiml.top_item();
  if(!top) return true; //it is ok if the file is empty.
  if(!solveInputDialog(*top)) return false;
  if(!solveCopy(top)) return false;
  if(!setSessionParameters(*top)) return false;
  if(!setAliases(*top)) return false;
  if(aParent) {
    // Look if already loaded !
    inlib::xml::tree* itemML = findFirstWidget(*top);
    if(itemML) {
      std::string name;
      itemML->attribute_value("name",name);
      if(name.empty()) {
        // create a component with a OnX_Component_<number> name.
      } else if(aParent->find_item(name)) {
        return true; // Already loaded.
      }
    }
  }
  // Create widgets, set hierarchy.
  if(aParent) top->set_parent(aParent);
  if(!createNativeWidget(*top)) return false;
  uiml.empty();
  if(aParent) aParent->add_child(top);
  if(aShow) showShells(*top);

  if(aExecCreate) {
    Execute tag(*this,"create");
    top->post_execute(executeItemMLCallbacks,&tag);
  }

  aTop = top;

  return true;
}

inlib::xml::tree* OnX::BaseUI::createItemML(
 const std::string& aName
,const std::string& aClass
,inlib::xml::tree* aParent
){
  std::vector<inlib::xml::tree::atb> atts(2);
  atts[0] = inlib::xml::tree::atb("class",aClass);
  atts[1] = inlib::xml::tree::atb("name",aName);
  inlib::xml::tree* itemML = fWidgetFactory.create("widget",atts,aParent);
  if(!itemML) return 0;
  if(aParent) aParent->add_child(itemML);
  return itemML;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::executeItemMLCallbacks(
 inlib::xml::tree& aItemML
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aItemML.tag_name()=="comment") return false;
  if(aItemML.tag_name()=="aliases") return false;
  OnX::Execute* tag = (OnX::Execute*)aTag;
  tag->baseUI().executeCallbacks(aItemML,tag->what());
  return true;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::BaseUI::buildName(
 inlib::xml::tree& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
// "OnX_Component_" is filtered in inlib::xml::tree::save.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string name;
  if(!aItemML.attribute_value("name",name)) { 
    name = createWidgetName();
    //FIXME : do it here ?
    aItemML.set_attribute_value("name",name);
  } else {
    // used in solveCopy. See ViewerWindow.onx.
    std::string sat;
    if(!aItemML.attribute_value("at",sat)) sat = "yes";
    if(sat=="yes") {
      std::string s;
      if(!atat(name,s)) {
        out() << "OnX::BaseUI::buildName :"
              << " atat on " << inlib::sout(name) << " failed."
              << std::endl;
      }
      name = s;
      aItemML.set_attribute_value("name",name);
    }
    if(!name.size()) {
      out() << "OnX::BaseUI::buildName :"
            << " empty widget name. Create a non empty one."
            << std::endl;
      name = createWidgetName();
      aItemML.set_attribute_value("name",name);
    }
  }
  return name;
}

bool OnX::BaseUI::createChildren(inlib::xml::tree& aItemML){
  // Create children :
  std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!createNativeWidget(*(*it))) return false;
  }
  return true;
}

bool OnX::BaseUI::setSessionParameters(inlib::xml::tree& aItemML){
  std::list<inlib::xml::element*> properties;
  aItemML.sub_elems(properties);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=properties.begin();it!=properties.end();++it) {
    if( ((*it)->name()=="session_parameter") ||
        ((*it)->name()=="variable") ){
      std::string name;
      if(!(*it)->attribute_value("name",name)) {
        out() << "BaseUI::setSessionParameters :" 
              << " <session_parameter>/<variable> without name attribute." 
              << std::endl;
        return false;
      }
      // by default we look also for a \n separated list of items.
      bool look_for_list = true;
     {std::string s;
      if((*it)->attribute_value("list",s)) {
        if(!inlib::to(s,look_for_list)) {
          out() << "BaseUI::setSessionParameters :" 
                << " for <variable> attribuet list not a boolean." 
                << std::endl;
          return false;
        }
      }}
      if(look_for_list) {
        std::string s = (*it)->value();
        inlib::replace(s,"\\n","\n");
        fSession.setParameter(name,s);
      } else {
        fSession.setParameter(name,(*it)->value());
      }
    }
  }

  // children :
 {std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!setSessionParameters(*(*it))) return false;
  }}

  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::setAliases(
 inlib::xml::tree& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fScriptManager) return false;

  if(aItemML.tag_name()=="aliases") {
    std::string sexec;
    if(!aItemML.attribute_value("exec",sexec)){
      out() << "BaseUI::setAliases :" 
            << " <aliases> without a exec attribute." << std::endl;
      //aItemML.dump_xml(printer(),"  ");
      return false;
    }

    Slash::UI::IInterpreter* interpreter = 
      fScriptManager->findInterpreter(sexec);
    if(!interpreter) {
      out() << "OnX::BaseUI::setAliases :"
            << " Unknown interpreter : " << sexec << std::endl;
      return false;
    }

    std::list<inlib::xml::element*> properties;
    aItemML.sub_elems(properties);
    std::list<inlib::xml::element*>::const_iterator it;
    for(it=properties.begin();it!=properties.end();++it) {
      if((*it)->name()=="alias") {

        std::string name;
        if(!(*it)->attribute_value("name",name)) {
          out() << "OnX::BaseUI::setAliases :"
                << " <alias> without a name attribute." 
                << std::endl;
          return false;
        }

        std::string value = (*it)->value();

        if(!interpreter->addAlias(name,value)) return false;

        if(fVerboseLevel) {
          out() << "OnX::BaseUI::setAliases :"
                << " for interpreter " << inlib::sout(sexec)
                << " set alias " << inlib::sout(name)
                << " with value " << inlib::sout(value)
                << std::endl;
        }
   
      }
    }       
  }



  // children :
 {std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!setAliases(*(*it))) return false;
  }}

  return true;

}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::solveInputDialog(
 inlib::xml::tree& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aItemML.tag_name()=="widget") {
    std::string sclass;
    if(!aItemML.attribute_value("class",sclass)){
      out() << "BaseUI::solveInputDialog :" 
            << " ItemML without a class." << std::endl;
      aItemML.dump_xml(out(),"  ");
      return false;
    }
    if(sclass=="InputDialog") {
      if(!createInputDialog(aItemML)) return false;
    }
  }

  // Solve children :
 {std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!solveInputDialog(*(*it))) return false;
  }}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::solveCopy(
 inlib::xml::tree*& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string scopy;
  if(aItemML->attribute_value("copy",scopy)) {
    if(!fTopItemML) {
      out() << "OnX::BaseUI::solveCopy :"
            << " topItemML not created." << std::endl;
      return false;
    }    
    std::string sat;
    if(!aItemML->attribute_value("at",sat)) sat = "yes";
    if(sat=="yes") {
      std::string s;
      if(!atat(scopy,s)) {
        out() << "OnX::BaseUI::solveCopy :"
              << " atat on " << inlib::sout(scopy) << " failed."
              << std::endl;
        return false;
      }
      scopy = s;
    }
    inlib::xml::tree* fromItemML = fTopItemML->find_item(scopy);
    if(!fromItemML) {
      out() << "OnX::BaseUI::solveCopy :"
            << " component " << inlib::sout(scopy) << " not found." 
            << std::endl;
      return false;
    }
    std::string sclass;
    if(!fromItemML->attribute_value("class",sclass)) {
      out() << "OnX::BaseUI::solveCopy :"
            << " ItemML to copy without a class." << std::endl;
      return false;
    }

    inlib::xml::tree* parent = aItemML->parent(); 
    if(!parent) {
      out() << "OnX::BaseUI::solveCopy :"
            << " ItemML without a parent." << std::endl;
      return false;
    }

    std::string name = buildName(*aItemML);

    inlib::xml::tree* new_itemML = createItemML(name,sclass,0);
    if(!new_itemML) {
      out() << "OnX::BaseUI::solveCopy :"
            << " can't create an ItemML of class "
            << inlib::sout(sclass) << "." 
            << std::endl;
      return false;
    }

    if(!new_itemML->copy(*fromItemML)) return false;
    new_itemML->set_attribute_value("name",name);

    parent->replace_child(aItemML,new_itemML);
    new_itemML->set_parent(parent);

    delete aItemML;
    aItemML = new_itemML;

    return true; //a copy must not have children.
  }

  // Solve children :
 {std::list<inlib::xml::tree*> list;
  aItemML->sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    inlib::xml::tree* item = const_cast<inlib::xml::tree*>(*it);
    if(!solveCopy(item)) return false;
  }}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::notification(
 Slash::Core::IMessage* aMessage
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fVerboseLevel) {
    out() << "OnX::BaseUI::notification :"
          << " receive message " << aMessage->message() << std::endl;
  }
  if(OnX::ExecuteMessage* mess = INLIB_CAST(*aMessage,OnX::ExecuteMessage)) {
    Action* action = mess->action();
    fCallbackData = action->callbackData();
    action->execute();
    fCallbackData = CallbackData();
    delete action;
  } else if(Lib::UpdateMessage* mess = 
              INLIB_CAST(*aMessage,Lib::UpdateMessage)) {
    std::string s = mess->what();
    //::printf("debug : UpdateMessage : \"%s\"\n",s.c_str());
    if(s=="*") {
      executeCallbacks("*","update");
    } else if(fTopItemML) {
      inlib::xml::tree* itemML = fTopItemML->find_item(s);
      if(!itemML) {
        out() << "OnX::BaseUI::notification :"
              << " UpdateMessage :"
              << " non existing component " << inlib::sout(s) << "."
              << std::endl;
      } else {
        executeCallbacks(*itemML,"update");
      }
    }
    Slash::Core::IRelation* relation = mess->relation();
    if(relation && fRelationManager && fRelationManager->isValid(*relation)) {
      //::printf("debug : UpdateMessage : relation unschedule\n");
      relation->unSchedule();
    }
  } else if(Lib::RelationMessage* mess = 
              INLIB_CAST(*aMessage,Lib::RelationMessage)) {
    Slash::Core::IRelation& relation = mess->relation();
    if(fRelationManager && fRelationManager->isValid(relation)) {
      relation.unSchedule();
    }
  }
  delete aMessage;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::removeCallback(
 const std::string& aName
,const std::string& aWhich
,int aIndex
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return false;
  inlib::xml::tree* itemML = fTopItemML->find_item(aName);
  if(!itemML) return false;
  OnX::Action::NativeWidget widget = itemML->get_data1();
  if(!widget) return false;
  Action* action = getAction(widget,aWhich,aIndex);
  if(!action) return false;
  deleteCallback(*itemML,aWhich,action);
  fActions.remove(action);
  delete action;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::setCallback(
 const std::string& aName
,const std::string& aWhich
,const std::string& aInterp
,const std::string& aScript
,int aIndex
)
//////////////////////////////////////////////////////////////////////////////
// If aIndex found override script.
// If not found add a new callback.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return false;
  inlib::xml::tree* itemML = fTopItemML->find_item(aName);
  if(!itemML) return false;

  if(aWhich=="destroy") { 
    // particular case of destroy which is not handled on Actions.
    bool found = false;
   {std::list<inlib::xml::element*> props;
    itemML->sub_elems(props);
    int index = 0;
    std::list<inlib::xml::element*>::const_iterator it;
    for(it=props.begin();it!=props.end();++it) {
      if(aWhich==(*it)->name()) {
        if(aIndex==index) {
          (*it)->set_attribute_value("exec",aInterp);
          (*it)->set_value(aScript);
          found = true;
          break;
        }
        index++;
      }
    }}
    if(!found) { // new callback.
      std::vector<inlib::xml::tree::atb> att;
      att.push_back(inlib::xml::tree::atb("exec",aInterp));
      itemML->add_element(aWhich,att,aScript);
    } else {
      itemML->set_element_atb_value(aWhich,"exec",aInterp,aIndex);
      itemML->set_element_value(aWhich,aScript,aIndex);
    }
    return true;
  }

  OnX::Action::NativeWidget widget = itemML->get_data1();
  if(!widget) return false;
  bool newCallback = false;
  Action* action = getAction(widget,aWhich,aIndex);
  if(!action) {
    newCallback = true;
    action = addCallback(*itemML,aWhich,aInterp,aScript);
  }
  if(!action) return false;
  action->setInterpreter(aInterp);
  action->setScript(aScript);
  // Update itemML :
  if (newCallback) {
    std::vector<inlib::xml::tree::atb> att;
    att.push_back(inlib::xml::tree::atb("exec",aInterp));
    itemML->add_element(aWhich,att,aScript);
  } else {
    itemML->set_element_atb_value(aWhich,"exec",aInterp,aIndex);
    itemML->set_element_value(aWhich,aScript,aIndex);
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::createIf(
 inlib::xml::tree& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string svalue;
  if(!aItemML.attribute_value("if",svalue)) return true;
  std::vector<std::string> words = inlib::words(svalue,".");
  if((words.size()==2) && (words[0]=="env")) {
    // if="env.<env>"           return true if <env> exists.
    // if="env.<env>=<value>"   return true if <env>==<value>
    std::vector<std::string> words2 = inlib::words(words[1],"=");
    if(words2.size()==1) { //if <env> exist.
      std::string value;
      return inlib::getenv(words2[0],value);
    } else if(words2.size()==2) { //if <env> == <value>
      std::string value;
      if(!inlib::getenv(words2[0],value)) return false;
      return (value==words2[1]);
    }
  } else if((words.size()==2) && (words[0]=="exec")) {
    // if="exec.<interp>"  return true if <interp> is operational.
    std::string sexec = words[1];
    if(!fScriptManager) return false;
    Slash::UI::IInterpreter* interpreter = 
      fScriptManager->findInterpreter(sexec);
    if(!interpreter) return false;
    return interpreter->isOperational();
  } else {
    // if="<param>"           return true if <param> is a session parameter.
    // if="<param>=<value>"   return true if <param>==<value>
    std::vector<std::string> words = inlib::words(svalue,"=");
    if(words.size()==1) {
      std::string value;
      return fSession.parameterValue(words[0],value);
    } else if(words.size()==2) {
      std::string value;
      if(!fSession.parameterValue(words[0],value)) return false;
      return (words[1]==value);
    }
  }
  return false;
}    
//////////////////////////////////////////////////////////////////////////////
inlib::xml::tree* OnX::BaseUI::findFirstWidget(
 inlib::xml::tree& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : stop descending under comment ?
  if(aItemML.tag_name()=="widget") return &aItemML;

  // Look children :
 {std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    inlib::xml::tree* itemML = findFirstWidget(*(*it));
    if(itemML) return itemML;
  }}
  return 0;
}

void OnX::BaseUI::removeInRelations(inlib::xml::tree& aItemML) {
  if(!fRelationManager) return;
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  fRelationManager->removeWith(widget);
}

void OnX::BaseUI::openFile(const std::string& aFile){
  // Used by the "desktop open file" mechanism.
  fSession.setParameter("session.openFile",aFile);
  executeCallbacks("*","openFile");
}

bool OnX::BaseUI::is_in(const inlib::xml::tree& aHead,inlib::xml::tree* aItemML){
  if(aItemML==&aHead) return true;
  std::list<inlib::xml::tree*> list;
  aHead.sub_trees(list);
  for(std::list<inlib::xml::tree*>::const_iterator it=
      list.begin();it!=list.end();++it) {
    if(is_in(*(*it),aItemML)) return true; //found
  }
  return false;
}

static bool saveItemML(
 inlib::xml::tree& aItemML
,const std::string& aFile
,int aVerboseLevel
,std::ostream& a_out
){
  if(aFile.empty()) return false;
  if(!aItemML.save_flag()) return true;
  std::string sfile;
  inlib::file_name(aFile,sfile);
  std::string name =  sfile + "_new";
  std::ofstream fout(name.c_str());
  if(fout.fail()) {
    a_out << "OnX::BaseUI::save :" 
          << " can't open file " << name << std::endl;
    return false;
  }
  if(aVerboseLevel) {
    a_out << "OnX::BaseUI::save :" 
          << " save file " << inlib::sout(sfile) << "..." << std::endl;
  }
  bool status = saveItemML(aItemML,fout,0,aVerboseLevel,a_out);
  fout.close();
  if(!status) {
    ::remove(name.c_str());
    if(aVerboseLevel) {
      a_out << "OnX::BaseUI::save :" 
            << " unable to save file " << inlib::sout(sfile) << "..." 
            << std::endl;
    }
  } else {
    // Replace file :
    inlib::file::mv(name,sfile);
  }
  return status;
}

static bool saveItemML(
 inlib::xml::tree& aItemML
,std::ostream& a_fout
,int aShift
,int aVerboseLevel
,std::ostream& a_out
){
  if(!aItemML.save_flag()) return true;
  
  std::string spaces;
  for(int i=0;i<aShift;i++) spaces += " ";

  const std::string& tagName = aItemML.tag_name();

  std::string sname;
  aItemML.attribute_value("name",sname);

  std::list<inlib::xml::tree*> childs;
  aItemML.sub_trees(childs);

  if(tagName=="reference") {
    std::string sfile;
    aItemML.attribute_value("file",sfile);
    a_fout << spaces 
         << "<reference"
         << " file="  << inlib::sout(sfile) << "/>" 
         << std::endl;
    
    // Save GUI associated to the reference :
    // Element should have one child :
    if(childs.size()!=1) return false;
    return saveItemML(*(childs.front()),sfile,aVerboseLevel,a_out);
  }

  if (aItemML.file() != "") {
    // If file field is present, we have to take care that file
    // is not already open by a parent.
    // (case of reference file (and others ?))
    inlib::xml::tree* parentItemML = aItemML.parent();
    if (parentItemML) {
      std::string pfile;
      parentItemML->attribute_value("file",pfile);
      if( (parentItemML->tag_name() == "reference") && 
          (pfile == aItemML.file()) ) {
      } else if (aItemML.file() == parentItemML->file()) {
      } else { 
        std::string file = aItemML.file();
        aItemML.set_file(""); //to avoid recursivity.
        bool status = saveItemML(aItemML,file,aVerboseLevel,a_out);
        aItemML.set_file(file);
        return status;
      }
    }
  }
  
  if(tagName=="OnX") { 
    // Is in principle at shift=0. 
    a_fout << spaces << "<OnX version=" << inlib::sout(ONX_VERSION) ;
  } else if(tagName=="aliases") {
    a_fout << spaces << "<aliases";
  } else if(tagName=="comment") {
    a_fout << spaces << "<comment";

    const std::vector<inlib::xml::tree::atb>& atbs = 
      aItemML.attributes();
    unsigned int atbn = atbs.size();
    for(unsigned int index=0;index<atbn;index++) {
      const std::string& atb = atbs[index].first;
      const std::string& atbv = atbs[index].second;
      if( (atb=="name") && isGeneratedName(atbv)) {
        // Do not save generated names.
      } else if(atb!="class") {
        a_fout << " " << atb << "=" << inlib::sout(atbv);
      }
    }

  } else { //FIXME : check that tagName is "widget".
    a_fout << spaces << "<widget";

    const std::vector<inlib::xml::tree::atb>& atbs = 
      aItemML.attributes();
    unsigned int atbn = atbs.size();
    for(unsigned int index=0;index<atbn;index++) {
      const std::string& atb = atbs[index].first;
      const std::string& atbv = atbs[index].second;
      if( (atb=="name") && (atbv.substr(0,14)=="OnX_Component_")) {
        // Do not save generated names.
      } else {
        a_fout << " " << atb << "=" << inlib::sout(atbv);
      }
    }

  }

  std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);

  if(!props.size() && !childs.size() ) {

    a_fout << "/>" << std::endl;

  } else {

    a_fout << ">" << std::endl;

    std::list<inlib::xml::element*>::const_iterator itp;
    for(itp=props.begin();itp!=props.end();++itp) {
      const std::vector<inlib::xml::tree::atb>& atbs = 
        (*itp)->attributes();

      bool isCallback = false;
      a_fout << spaces << "  <" << (*itp)->name();
      unsigned int atbn = atbs.size();
      for(unsigned int index=0;index<atbn;index++) {
        const std::string& atb = atbs[index].first;
        const std::string& atbv = atbs[index].second;
        a_fout << " " << atb << "=" << inlib::sout(atbv);
        if(atb=="exec") isCallback = true;
      }
      if((*itp)->value().empty()) {
        a_fout << "/>" << std::endl;
      } else {
        a_fout << ">";
        std::string value = inlib::to_xml((*itp)->value());
        if(isCallback) {
          if(value.find("\\n\\")==std::string::npos) {
            a_fout << value;
          } else {
            a_fout << std::endl;
            inlib::replace(value,"\\n\\","@OnX@");
            inlib::replace(value,"@OnX@","\\n\\\n");
            inlib::strip(value,inlib::trailing,' ');
            a_fout << value;
            unsigned int l = value.size();
            if(l && value[l-1]!='\n') a_fout << std::endl;
            a_fout << spaces << "  ";
          }
        } else {
          a_fout << value;
        }
        a_fout << "</" << (*itp)->name() << ">" << std::endl;
      }
    }

    if(tagName!="InputDialog") {
      //Children :
      std::list<inlib::xml::tree*>::const_iterator it;
      for(it=childs.begin();it!=childs.end();++it) {
        if(!saveItemML(*(*it),a_fout,aShift+2,aVerboseLevel,a_out)) 
          return false;
      }
    }  
  
    a_fout << spaces << "</" << tagName << ">" << std::endl;

  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Slash::UI::ICyclic* OnX::BaseUI::findCyclic(const std::string& aName) const {
  std::list<Slash::UI::ICyclic*>::const_iterator it;
  for(it=fCyclics.begin();it!=fCyclics.end();++it) {
    if((*it)->name()==aName) return *it;
  }
  return 0;
}

bool OnX::BaseUI::removeCyclic(Slash::UI::ICyclic*& aCyclic){
  fCyclics.remove(aCyclic);
  delete aCyclic;
  aCyclic = 0;
  return true;
}

void OnX::BaseUI::removeCyclics(){
  std::list<Slash::UI::ICyclic*>::iterator it;
  for(it=fCyclics.begin();it!=fCyclics.end();it = fCyclics.erase(it)) 
    delete (*it);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX::BaseUI::steerModal(){while(steer()!=ONX_BREAK_MODAL) {}}
void OnX::BaseUI::exitModal(){exit(ONX_BREAK_MODAL);}

std::string OnX::BaseUI::ask(const std::string& aPrompt){
  if(!fTopItemML) return "";
  inlib::xml::tree* parent = fTopItemML->find_by_attribute("class","Window");
  if(!parent) {
    out() << "OnX::BaseUI::ask :"
          << " can't find a Window widget to attach the AskDialog."
          << std::endl;
    return "";
  }
  inlib::xml::tree* top;
  if(!load(parent,"$ONXROOT/scripts/OnX/AskDialog.onx",true,true,true,top)){
    out() << "OnX::BaseUI::ask :"
          << " can't load the AskDialog."
          << std::endl;
    return "";
  }
  setParameter("OnX_AskDialog_prompt.label",aPrompt);
  setParameter("OnX_AskDialog_entry.value","");
  Slash::UI::IWidget* w = findWidget("OnX_AskDialog_dialog");
  if(w) w->show();
  steerModal();
  //echo("debug : exit steer loop.");
  std::string value;
  parameterValue("OnX_AskDialog_entry.value",value);
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::askString :"
        << value
        << std::endl;
  }
  return value;
}

//////////////////////////////////////////////////////////////////////////////
// Laurent ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
std::vector< std::pair<bool,std::string> > OnX::BaseUI::parseDecorations(
 const std::string& aDecoration
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector< std::pair<bool,std::string> > decos;
  std::vector<std::string> items;
  smanip_lines(aDecoration,items); //FIXME
  unsigned int itemn = items.size();

  //look the first, if it is not all or none, we go to the next
  bool close_button = true;
  bool iconify_button = true;
  bool min_max_button = true;
  bool resize_handle = true;
  bool border = true;
  bool caption = true;
  bool system_menu = true;
  if (itemn >0) {
    if (items[0]=="all"){ // do nothing 
      close_button = true;
    } else if (items[0]=="none"){ // remove all
      close_button = false;
      iconify_button = false;
      min_max_button = false;
      resize_handle = false;
      border = false;
      caption = false;           
      system_menu = false;
    }

    // To have a icon, close, or minimize-maximize button, you should
    // have a caption
    // If you have a system menu, you will also have a close button

    for(unsigned int index=0;index<itemn;index++){
      if (items[index]=="no_close_button")        close_button = false;
      else if (items[index]=="close_button")      close_button = true;
      else if (items[index]=="no_iconify_button") iconify_button = false;
      else if (items[index]=="iconify_button")    iconify_button = true;
      else if (items[index]=="no_system_menu")    system_menu = false;
      else if (items[index]=="system_menu")       system_menu = true;
      else if (items[index]=="no_min_max_button") min_max_button = false;
      else if (items[index]=="min_max_button")    min_max_button = true;
      else if (items[index]=="no_resize_handle")  resize_handle = false;
      else if (items[index]=="resize_handle")     resize_handle = true;
      else if (items[index]=="no_border")         border = false;
      else if (items[index]=="border")            border = true;
      else if (items[index]=="no_caption")        caption = false;
      else if (items[index]=="caption")           caption = true;
      else if (items[index]=="all") {}
      else if (items[index]=="none") {}
      else if (items[index].size()) 
        out() << "OnX::BaseUI::parseDecorations :"
            << " WARNING : The decoration " << inlib::sout(items[index]) 
            << " is not recognised." 
            << " Legal options are "
            << " none"
            << " [no_]close_button"
            << " [no_]iconify_button" 
            << " [no_]system_menu" 
            << " [no_]min_max_button [no_]resize_handle [no_]border"
            << " [no_]caption all none." 
            << std::endl; 
    }
    if (!caption && 
        ((close_button )|| (iconify_button) || 
         (min_max_button) || (system_menu)))
      out() << "OnX::BaseUI::parseDecorations :" 
          << " WARNING : You want to create a widget whithout caption, " 
          << "but you should have a caption to have a"
          << " close,iconify,system menu " 
          << "or min max button" 
          << std::endl;
  }
  decos.push_back(std::pair<bool,std::string>(close_button,"close_button"));
  decos.push_back(std::pair<bool,std::string>(iconify_button,"iconify_button"));
  decos.push_back(std::pair<bool,std::string>(min_max_button,"min_max_button"));
  decos.push_back(std::pair<bool,std::string>(resize_handle,"resize_handle"));
  decos.push_back(std::pair<bool,std::string>(border,"border"));
  decos.push_back(std::pair<bool,std::string>(caption,"caption"));
  decos.push_back(std::pair<bool,std::string>(system_menu,"system_menu"));
  return decos;
}
//////////////////////////////////////////////////////////////////////////////
// current widget ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Slash::UI::IWidget* OnX::BaseUI::currentWidget(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCurrentWidget;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::setCurrentWidget(
 Slash::UI::IWidget* aWidget
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    if(aWidget) {
      out << "OnX::BaseUI::setCurrentWidget :"
          << " " << inlib::sout(aWidget->name())
          << std::endl;
    } else {
      out << "OnX::BaseUI::setCurrentWidget :"
          << " to null."
          << std::endl;
    }
  }
  fCurrentWidget = aWidget;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
// terminal //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::enableTerminal(
 const std::string& //aPrompt
,const std::string& //aInterp
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::disableTerminal(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::redirectAskToTerminal(
 bool aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAskToTerminal = aValue;
  return true;
} 
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::lockScreen(
 const std::string&
,const std::string&
,const std::string&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false;
} 

#include "InputDialog.cxx"
#include "IB.cxx"

bool treat_atat(Slash::UI::IUI&,const std::string&,std::string&);

bool OnX::BaseUI::atat(const std::string& aString,std::string& aValue) const{
  Slash::UI::IUI* ui = (Slash::UI::IUI*)this;
  return treat_atat(*ui,aString,aValue);
}

bool OnX::BaseUI::netSteer(){
  if(!fNetManager) return false;

  inlib::net::base_socket& socket = fNetManager->socket();

  bool have_input;
  if(!socket.is_there_input(have_input)) {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::netSteer :"
        << " is_there_input failed"
        << std::endl;
    //socket.disconnet();
    return false;
  }
  if(!have_input) return true; //ok nothing to be done.
  
  std::string prot;
  if(!socket.fetch_string(prot)) {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::nativeSteer :"
        << " fetch_string failed."
        << std::endl;
    //socket.disconnet();
    return false;
  }

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::BaseUI::netSteer : "
        << " from client protocol " << inlib::sout(prot)
        << std::endl;
  }

  if(prot==inlib_net_protocol_doc) {
    std::string DOCUMENT;
    if(!inlib::tmpname("","OnX_server_","",DOCUMENT)) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::nativeSteer :"
          << " can't get tmp file name."
          << std::endl;
      //socket.disconnet();
      return false;
    }
  
    std::string viewer;
    if(!socket.fetch_string(viewer)) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::nativeSteer :"
          << " fetch_string failed."
          << std::endl;
      //socket.disconnet();
      return false;
    }

    std::string placement;
    if(!socket.fetch_string(placement)) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::nativeSteer :"
          << " fetch_string failed."
          << std::endl;
      //socket.disconnet();
      return false;
    }

    if(!socket.fetch_file(DOCUMENT)) {
      std::ostream& out = fSession.cout();
      out << "OnX::BaseUI::nativeSteer :"
          << " fetch_file failed."
          << std::endl;
      //socket.disconnet();
      return false;
    }

    std::string where = "@current@"; //FIXME : get from client.
    receiveScene(where,placement,DOCUMENT);
     ::remove(DOCUMENT.c_str());

  }
  return true;
}

