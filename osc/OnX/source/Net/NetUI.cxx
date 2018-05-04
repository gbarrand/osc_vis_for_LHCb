// this :
#include <OnX/Net/NetUI.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IScriptManager.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Tools/Manager.h>
#include <Slash/Tools/Data.h>
#include <Slash/Net/protocol>

#include <inlib/system>
#include <inlib/file>
#include <inlib/args>

#include <inlib/cast>

#include <cstdlib> //exit
#include <cstdio> //printf

#define ONX_HAS_INVENTOR
#ifdef ONX_HAS_INVENTOR
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <HEPVis/nodekits/SoPage.h>
#include <Slash/Tools/ClientSceneHandler.h>
#include <Slash/Tools/names.h>
#include <OnX/Inventor/Inventor.h>
#include <OnX/Inventor/HEPVis.h>
#endif

#include <Slash/UI/IWidget.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/UI/IConsole.h>
#include <OnX/Core/WidgetClass.h>

namespace OnX {
namespace Net {

class SoViewer
:public virtual Slash::UI::IWidget 
,public virtual Slash::UI::ISoViewer
{
public: //Slash::UI::IWidget
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::UI::IWidget)
    else INLIB_IF_CAST(Slash::UI::ISoViewer)
    else return 0;
  }
  virtual const Slash::UI::IWidgetClass& widgetClass() const {
    return fWidgetClass;
  }
  virtual std::string name() const {return fName;}
  virtual void* nativeWidget() const {return 0;}
  virtual bool executeCallbacks(const std::string&) {return false;}
  virtual bool size(unsigned int&,unsigned int&) {return false;}
  virtual bool position(int&,int&) {return false;}
  virtual bool write(const std::string& file,
                     const std::string& format,
                     const std::string& options = "") {return false;}
  virtual bool setParameter(const std::string&,const std::string&) {return false;}
  virtual bool parameterValue(const std::string&,std::string&) {return false;}
  virtual bool manage(bool) {return false;}
  virtual bool refresh() {return false;}
  virtual bool show() {return false;}
  virtual bool hide() {return false;}

public: //Slash::UI::ISoViewer
  virtual void setBackgroundColor(double red,double green,double blue) {}
  virtual void setDecoration(bool) {}
  virtual void setViewing(bool) {}
  virtual void setAutoClipping(bool) {}
  virtual void setHeadlight(bool) {}
  virtual void setSmoothing(bool) {}
  virtual void setTitle(const std::string&) {}
  virtual void render() {}
  virtual void viewAll() {}
  virtual bool readScene(const std::string&,
                         const std::string&,
                         const std::string&){
    return false;
  }
  virtual bool clear(const std::string&){return false;}
  virtual bool removeManips(const std::string&) {return false;}
  virtual bool detectIntersections(bool stopAtFirst,
                                   const std::string& aOptions) {return false;}
  virtual void setFeedbackVisibility(bool) {}
  virtual void setAnimation(bool) {}
  virtual void stopAnimating() {}
  virtual SoNode* nodeOfInterest(const std::string& aWhat) const {return 0;}
  virtual SoNode* sceneGraph() const {return fSceneGraph;}
  virtual SbViewportRegion* viewportRegion() const {return 0;}
  virtual bool collect(int,int,unsigned int,unsigned int) {return false;}
  virtual SoGLRenderAction* getGLRenderAction() const {return 0;}
  virtual void setGLRenderAction(SoGLRenderAction*) {}
  virtual bool setTransparencyType(int) {return false;}
  virtual bool transparencyType(int& a_type) const {a_type = 0;return false;}
public:
  SoViewer(const std::string& aName = "")
  :fName(aName)
  ,fWidgetClass("PageViewer",
                std::vector<std::string>(),
                std::vector<std::string>(),
                std::vector<std::string>(),
                std::vector<std::string>())
  ,fSceneGraph(0)
  {}
  virtual ~SoViewer(){
    if(fSceneGraph) fSceneGraph->unref();
  }
public:
  void setSceneGraph(SoNode* aNode) {
    if(fSceneGraph) return; //done.
    fSceneGraph = aNode;
  }
protected:
  std::string fName;
  OnX::WidgetClass fWidgetClass;
  SoNode* fSceneGraph;
};

}}

#include <sstream>

namespace OnX {
namespace Net {

class Console
:public virtual Slash::UI::IWidget 
,public virtual Slash::UI::IConsole
,public std::streambuf
{
public: //Slash::UI::IWidget
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::UI::IWidget)
    else INLIB_IF_CAST(Slash::UI::IConsole)
    else return 0;
  }
  virtual const Slash::UI::IWidgetClass& widgetClass() const {
    return fWidgetClass;
  }
  virtual std::string name() const {return fName;}
  virtual void* nativeWidget() const {return 0;}
  virtual bool executeCallbacks(const std::string&) {return false;}
  virtual bool size(unsigned int&,unsigned int&) {return false;}
  virtual bool position(int&,int&) {return false;}
  virtual bool write(const std::string& file,
                     const std::string& format,
                     const std::string& options = "") {return false;}
  virtual bool setParameter(const std::string&,const std::string&) {return false;}
  virtual bool parameterValue(const std::string&,std::string&) {return false;}
  virtual bool manage(bool) {return false;}
  virtual bool refresh() {return false;}
  virtual bool show() {return false;}
  virtual bool hide() {return false;}

public: //Slash::UI::IConsole
  virtual std::ostream& out() {return f_out;}
  virtual bool beginOut() {
    fAccum.clear();
    return true;
  }
  virtual bool endOut() {
    fClient.send_string(inlib_net_protocol_out);
    fClient.send_string(fAccum);
    fAccum.clear();
    return true;
  }
public:
  inline Console(inlib::net::base_socket& aClient,
                 const std::string& aName = "")
  :fName(aName)
  ,fWidgetClass("Console",
                std::vector<std::string>(),
                std::vector<std::string>(),
                std::vector<std::string>(),
                std::vector<std::string>())
  ,fClient(aClient)
  ,f_out(this)
  ,fBuffer(0)
  ,fSize(4095)
  ,fCount(0)
  {
    fBuffer = new char[fSize+1];
  }
  virtual ~Console() {delete [] fBuffer;}
private:
  inline Console(const Console& aFrom)
  :fWidgetClass(aFrom.fWidgetClass),fClient(aFrom.fClient)
  ,f_out(this){}
  inline Console& operator=(const Console& aFrom){return *this;}
public:
  inline int overflow(int aChar) {
    int result = 0;
    if(fCount>=fSize) {
      fBuffer[fCount] = '\0';
      fCount = 0;
      result = receiveString();
    }
    fBuffer[fCount] = aChar;
    fCount++;
    if(aChar=='\n') {
      fBuffer[fCount] = '\0';
      fCount = 0;
      result = receiveString();
    }
    return result;
  }

  inline int sync() {
    fBuffer[fCount] = '\0';
    fCount = 0;
    return receiveString ();
  }

#ifdef WIN32
  inline int underflow() {return 0;}
#endif
private:
  int receiveString() {
    //std::string stringToSend = fBuffer;
    //fClient.send_string(inlib_net_protocol_out);
    //fClient.send_string(stringToSend);
    fAccum += fBuffer;
    return 0;
  }
protected:
  std::string fName;
  OnX::WidgetClass fWidgetClass;
  inlib::net::base_socket& fClient;
  std::ostream f_out;
  char* fBuffer;
  unsigned int fSize;
  unsigned int fCount;
  std::string fAccum;
};

}}

OnX::NetUI::NetUI(Slash::Core::ISession& aSession,const std::vector<std::string>& /*aArgs*/)
:fSession(aSession)
,fScriptManager(0)
,fVerboseLevel(0)
,fIsValid(false)
,fClient(aSession.cout(),fVerboseLevel)
,fCurrentWidget(0)
,fBeingDestroyed(false)
,fToExit(false)
,fEnableOutput(false)
{
  fScriptManager = Slash_findManager(fSession,"ScriptManager",Slash::UI::IScriptManager);

  std::string svalue;
  if(fSession.parameterValue("UI_Manager.verbose",svalue)) {
    int value;
    if(inlib::to<int>(svalue,value)) fVerboseLevel = value;
  } else {
    fVerboseLevel = fSession.verboseLevel();
  }

  inlib::args args(fSession.arguments());

  std::string server;
  if(!args.find("-server",server)) {
    if(!inlib::getenv("OSC_SERVER",server)) {
      std::ostream& out = fSession.cout();
      out << "OnX::NetUI::NetUI : "
          << " serveur host not specified."
          << " -server=<string> and OSC_SERVER not defined."
          << std::endl;
      return;
    }
  }

  unsigned int port;
  if(!args.find<unsigned int>("-port",port)) port = 50800;

  if(!fClient.connect(server,port,10,1)) {
    std::ostream& out = fSession.cout();
    out << "OnX::NetUI::NetUI : "
        << " can't connect to server."
        << std::endl;
    return;
  }

 {std::string prot;
  if(!fClient.fetch_string(prot)) {
    std::ostream& out = fSession.cout();
    out << "OnX::NetUI::NetUI : "
        << " can't send driver string to server."
        << std::endl;
    return;
  }
  if(prot!=inlib_net_protocol_hello) {
    std::ostream& out = fSession.cout();
    out << "OnX::NetUI::NetUI : "
        << " expected the hello protocol."
        << " Received " << inlib::sout(prot)
        << std::endl;
    return;
  }}

 {std::string driver;
  args.find("-server_toolkit",driver);
  if(!fClient.send_string(driver)) {
    std::ostream& out = fSession.cout();
    out << "OnX::NetUI::NetUI : "
        << " can't send driver string to server."
        << std::endl;
    return;
  }}

#ifdef ONX_HAS_INVENTOR
  if(!SoDB::isInitialized()) {SoDB::init();}
  SoNodeKit::init();
  //SoInteraction::init();

  HEPVis::initClasses();
  OnX::Inventor::initialize(aSession);

  if(!aSession.findManager(Slash_SceneHandler))  {
    aSession.addManager
      (new Slash::ClientSceneHandler(aSession,Slash_SceneHandler,fClient));
  }
#endif

  // Create a current widget :
 {OnX::Net::SoViewer* soViewer = new OnX::Net::SoViewer("Viewer");
  fWidgets.push_back(soViewer);
#ifdef ONX_HAS_INVENTOR
  SoPage* soPage = new SoPage();
  soPage->ref();
  soPage->createRegions("SoDisplayRegion",1,1,0);
  soViewer->setSceneGraph(soPage); //soViewer becomes the owner.
#endif
  fCurrentWidget = soViewer;}

  //for OnXLab / AIDA :
 {OnX::Net::SoViewer* soViewer = new OnX::Net::SoViewer("Plotter");
  fWidgets.push_back(soViewer);
#ifdef ONX_HAS_INVENTOR
  SoPage* soPage = new SoPage();
  soPage->ref();
  //soPage->createRegions("SoDisplayRegion",1,1,0);
  soViewer->setSceneGraph(soPage); //soViewer becomes the owner.
#endif
 }

 {OnX::Net::Console* console = 
     new OnX::Net::Console(fClient,"consoleText");
  fWidgets.push_back(console);}

  fIsValid = true;
}

OnX::NetUI::~NetUI() { 
  fEnableOutput = false;
  fBeingDestroyed = true;

  std::vector<Slash::UI::IWidget*>::iterator it;
  for(it=fWidgets.begin();it!=fWidgets.end();++it) delete (*it);
  fWidgets.clear();
  fCurrentWidget = 0;

  fClient.disconnect();

#ifdef ONX_HAS_INVENTOR
  OnX::Inventor::finalize(fSession);
#endif
}

int OnX::NetUI::steerLevel() const {return 0;}
void OnX::NetUI::steerModal() {}
void OnX::NetUI::exitModal() {}

std::ostream& OnX::NetUI::out() const {return fSession.out();}

void* OnX::NetUI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(Slash::UI::IUI)
  else return 0;
}

void OnX::NetUI::put(const std::string& aString){
  // Append line at end of report region.
  if(fEnableOutput) {
    // send the output to the server :
    fClient.send_string(inlib_net_protocol_out);
    fClient.send_string(aString);
  } else {
    ::printf("%s",aString.c_str());
  }
}

bool OnX::NetUI::executeScript(
 const std::string& aInterp
,const std::string& aScript
,const Slash::UI::IUI::Options& aParams
){
  if(!fScriptManager) return false;
  return fScriptManager->executeScript(aInterp,aScript,aParams);
}

int OnX::NetUI::steer(){
  if(!fIsValid) return 0;
  // Treat events coming from the OnX server :

  while(true) {

    if(fToExit) {
      //std::cout << "G4Lab::ClientUI::steer :"
      //          << " fToExit true. break."
      //          << std::endl;       
      fToExit = false;
      break;
    }

    bool have_input;
    if(!fClient.is_there_input(have_input)) {
      std::ostream& out = fSession.cout();
      out << "OnX::NetUI::steer : "
          << std::endl;       
      return 0;
    }
    if(!have_input) continue;

    std::string prot;

    if(!fClient.fetch_string(prot)) return 0;

    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "OnX::NetUI::steer : "
          << " from server protocol " << inlib::sout(prot)
          << std::endl;
    }

    if(prot==inlib_net_protocol_out_ready) {

      //fEnableOutput = true; //avoid G4 send all output to server.

    } else if(prot==inlib_net_protocol_file) {
      std::string file;
      if(!fClient.fetch_string(file)) return 0;
      // Exa <reference> .onx file 
      std::string p;
      inlib::file_name(file,p);
      if(!fClient.send_file(p)) {
        std::ostream& out = fSession.cout();
        out << "OnX::NetUI::steer : "
            << " can't send file " << inlib::sout(file) << "."
            << std::endl;
        return 0;
      }

    } else if(prot==inlib_net_protocol_file_exists) { 

      std::string file;
      if(!fClient.fetch_string(file)) return 0;
      std::string p;
      inlib::file_name(file,p);
      bool value = inlib::file::exists(p);
      if(!fClient.send_string(value?"true":"false")) return 0;

    } else if(prot==inlib_net_protocol_exit) {
      std::string scode;
      if(!fClient.fetch_string(scode)) return 0;
      int exit_code;
      if(!inlib::to<int>(scode,exit_code)) return 0;
      return exit_code;

    } else if(prot==inlib_net_protocol_cbk) {

      std::string cbk;
      if(!fClient.fetch_string(cbk)) return 0;

      std::vector<std::string> words;
      inlib::words(cbk," ",false,words);
      if(words.size()<2) {
        std::ostream& out = fSession.cout();
        out << "OnX::NetUI::steer : "
            << " bad syntax in " << inlib::sout(cbk) 
            << std::endl;
      } else {
        std::string interp = words[0];
        words.erase(words.begin());
        std::string script = inlib::tos<std::string>(words," ");

        if(fVerboseLevel) {
          std::ostream& out = fSession.cout();
          out << "OnX::NetUI::steer : "
              << " interp " << inlib::sout(interp)
              << " script " << inlib::sout(script)
              << std::endl;
        }

        if(fScriptManager) {
          Slash::UI::IInterpreter::Options params;
          fScriptManager->executeScript(interp,script,params);
        }
      }

    } else if(prot==inlib_net_protocol_selection) {
      std::string sdata;
      if(!fClient.fetch_string(sdata)) return 0;
      Slash::Data::init(fSession,sdata);

    } else {
       std::ostream& out = fSession.cout();
       out << "OnX::NetUI::steer : "
           << " receive unknown protocol : " << inlib::sout(prot) 
           << " from server."
           << std::endl;
    }

  }

  return 0;
}

void OnX::NetUI::synchronize(){}

void OnX::NetUI::exit(int){
  fToExit = true;
}

void OnX::NetUI::echo(const std::string& aString){
  std::ostream& out = fSession.cout();
  out << aString << std::endl;
}

void OnX::NetUI::warn(const std::string& aString){
  std::ostream& out = fSession.cout();
  out << "WARNING : " << aString << std::endl;
}

std::string OnX::NetUI::ask(const std::string&){return "";}

void OnX::NetUI::addDispatcher(Slash::UI::IUI::Dispatcher){}

bool OnX::NetUI::removeCallback(const std::string&,const std::string&,int){
  return false;
}

bool OnX::NetUI::setCallback(
 const std::string&
,const std::string&
,const std::string&
,const std::string&
,int
){
  return false;
}

std::string OnX::NetUI::callbackValue(){return fValue;}

const std::string& OnX::NetUI::thisComponent(){return fValue;}

Slash::UI::IWidget* OnX::NetUI::findWidget(const std::string& a_string) const {
  std::vector<Slash::UI::IWidget*>::const_iterator it;
  for(it=fWidgets.begin();it!=fWidgets.end();++it) {
    if((*it)->name()==a_string) return *it;
  }
  return 0;
}

Slash::UI::IWidget* OnX::NetUI::findWidgetByAttribute(const std::string& aAttribute,const std::string& aValue,bool,bool) const {return 0;}

bool OnX::NetUI::load(Slash::UI::IWidget& aParent,const std::string& aToLoad,bool aIsFile,bool aShow) {return false;}

Slash::UI::IWidget* OnX::NetUI::createWidget(
 const Slash::UI::IWidgetClass&
,const std::string&
,Slash::UI::IWidget&
,const std::vector<std::string>&
){
  return 0;
}

bool OnX::NetUI::parameterValue(const std::string&,std::string& aValue) {
  aValue = "";
  return false;
}

bool OnX::NetUI::setParameter(const std::string&,const std::string&){return false;}

bool OnX::NetUI::load(const std::string& aParent,const std::string& aFile,bool,bool){
  if(!fClient.send_string(aFile)) {
    std::ostream& out = fSession.cout();
    out << "OnX::NetUI::NetUI : "
        << " can't send OnX file " << aFile << " to server."
        << std::endl;
    return false;
  }
  return true;
}

void OnX::NetUI::deleteChildren(const std::string&){}

Slash::UI::ICyclic* OnX::NetUI::createCyclic(const std::string&,int,const std::string&,const std::string&){return 0;}

bool OnX::NetUI::createComponent(const std::string&,const std::string&,const std::string&){return false;}

std::vector<std::string> OnX::NetUI::findChildren(const std::string&,const std::string&,const std::string&) const {return std::vector<std::string>();}

std::vector<std::string> OnX::NetUI::findParent(const std::string&,const std::string&,const std::string&) const {return std::vector<std::string>();}

bool OnX::NetUI::destroyComponent(const std::string&){return false;}

bool OnX::NetUI::executeCallbacks(const std::string&,const std::string&){return false;}

bool OnX::NetUI::copyComponent(const std::string&,const std::string&,const std::string&){return false;}

Slash::UI::IWidgetClass* OnX::NetUI::findWidgetClass(const std::string&) const {return 0;}

bool treat_atat(Slash::UI::IUI&,const std::string&,std::string&);

bool OnX::NetUI::atat(const std::string& aString,std::string& aValue) const{
  Slash::UI::IUI* ui = (Slash::UI::IUI*)this;
  return treat_atat(*ui,aString,aValue);
}


