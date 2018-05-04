#ifndef OnX_BaseUI_h
#define OnX_BaseUI_h 

// Inheritance :
#include <Slash/UI/IUI.h>
#include <Slash/UI/IIB.h> //it has forward cast to inlib::xml::tree

#include <OnX/Core/CallbackData.h>
#include <OnX/Core/WidgetFactory.h>
#include <OnX/Core/Action.h> //FIXME NativeWidget

#include <list>

namespace Slash {namespace UI {class IScriptManager;}}
namespace Slash {namespace UI {class IWidgetFactory;}}
namespace Slash {namespace Net {class INetManager;}}

namespace Slash {namespace Core {class IRelationManager;}}

namespace exlib {namespace xml {class file_getter;}}
namespace exlib {namespace xml {class tree;}}

namespace OnX {

//FIXME class Action;
class BaseUI;

class Execute {
public:
  Execute(BaseUI&,const std::string&);
  const std::string& what() const;
  BaseUI& baseUI();
private:
  BaseUI& fUI;
  std::string fWhat;
};

class BaseUI 
:public virtual Slash::UI::IUI
,public virtual Slash::UI::IIB 
{
public: //Slash::UI::IUI
  virtual void* cast(const std::string&) const;
  virtual bool isValid() const;
  virtual int steer();
  virtual void steerModal();
  virtual void exitModal();
  virtual bool executeScript(const std::string&,const std::string&,const Slash::UI::IUI::Options& = Slash::UI::IUI::Options());
  virtual void addDispatcher(Slash::UI::IUI::Dispatcher);
  virtual bool setCallback(const std::string&,
                           const std::string&,
                           const std::string&,
                           const std::string&,
                           int = 0);
  virtual bool removeCallback(const std::string&,
                              const std::string&,
                              int = 0);
  virtual std::string callbackValue();
  virtual const Slash::UI::ICallbackData& callbackData();
  virtual bool setCallbackData(const Slash::UI::ICallbackData&);
  virtual bool executeCallbacks(const std::string&,const std::string&);
  virtual bool load(const std::string&,const std::string&,bool is_file = true,bool show = true);
  virtual bool save();
  virtual void deleteChildren(const std::string&);
  virtual void echo(const std::string&);
  virtual std::string ask(const std::string&);

  virtual bool show();

  //IWidget API:
  virtual Slash::UI::IWidgetClass* findWidgetClass(const std::string&) const;
  virtual Slash::UI::IWidget* findWidget(const std::string&) const;
  virtual Slash::UI::IWidget* createWidget(const Slash::UI::IWidgetClass&,
                                const std::string&,
                                Slash::UI::IWidget&,
                                const std::vector<std::string>& = std::vector<std::string>());
  virtual bool destroyWidget(Slash::UI::IWidget*&);
  virtual Slash::UI::IWidget* findWidgetByAttribute(const std::string& atb,
                                                    const std::string& value,
                                                    bool ud = true,
                                                    bool lr = true) const;
  virtual bool load(Slash::UI::IWidget& parent,const std::string&,bool is_file = true,bool show = true);
  virtual Slash::UI::IWidget* currentWidget() const; 
  virtual bool setCurrentWidget(Slash::UI::IWidget*); 

  //"by name" API:
  virtual bool createComponent(const std::string&,
                               const std::string&,
                               const std::string&);
  virtual bool destroyComponent(const std::string&);
  virtual bool copyComponent(const std::string&,
                             const std::string&,
                             const std::string&);
  virtual std::vector<std::string> findChildren(const std::string&,
                                                const std::string&,
                                                const std::string&) const;
  virtual std::vector<std::string> findParent(const std::string&,
                                              const std::string&,
                                              const std::string&) const;
  virtual Slash::Core::ISession& session();
  virtual Slash::UI::ICyclic* findCyclic(const std::string&) const;
  virtual bool removeCyclic(Slash::UI::ICyclic*&);

  virtual std::string createWidgetName();

  virtual bool isComponent(const std::string&);
  virtual int steerLevel() const;

  virtual bool beingDestroyed() const;
  virtual bool finalize();

  virtual bool enableTerminal(const std::string&,const std::string&);
  virtual bool disableTerminal();
  virtual bool redirectAskToTerminal(bool); 
  virtual bool lockScreen(const std::string& passwd,
                          const std::string& interp = "",
                          const std::string& script = ""); 
  virtual bool atat(const std::string&,std::string&) const;

  //DEPRECATED:
  virtual bool show(const std::string&);
public: //Slash::UI::IIB  
  virtual std::string tree() const;
  virtual std::vector< std::pair<bool,std::string> > parseDecorations(const std::string&) const;
  virtual bool propertyType(const std::string&,std::string&) const;
  virtual const std::vector<std::string>& callbackLanguageList() const;

  virtual bool loadReferenceFile(inlib::xml::tree&, const std::string&);
  virtual inlib::xml::tree* topItemML() const;

public:
  bool notification(Slash::Core::IMessage*);
  // Used in QtUI :
  void setCallbackData(const CallbackData&);
  // Used in Hippo :
  inlib::xml::factory& itemMLFactory();
  void addFactory(Slash::UI::IWidgetFactory*);
  std::ostream& out() const;
  static bool executeItemMLCallbacks(inlib::xml::tree&,void*);
  //
  void addWidgetClass(Slash::UI::IWidgetClass*);
  // Used in gl2ps_proxy :
  bool executeCallbacks(inlib::xml::tree&,const std::string&);
  // Used by the "desktop open file" system :
  void openFile(const std::string&);
protected:
  BaseUI(Slash::Core::ISession&);
  BaseUI(const BaseUI&);
  virtual ~BaseUI();
private:
  BaseUI& operator=(const BaseUI&);
protected:
  // REVIST : should have some IDevUI.h for the below.
  virtual int nativeSteer() = 0;
  virtual bool createNativeWidget(inlib::xml::tree&,
                            const std::string&, //class
                            const std::string&, //name
                            inlib::xml::tree* //parent
                            ) = 0;
  virtual bool destroyNativeWidget(inlib::xml::tree&) = 0;
  virtual Action* addCallback(inlib::xml::tree&,
                              const std::string&,
                              const std::string&,
                              const std::string&) = 0;
  virtual void deleteCallback(inlib::xml::tree&,
                              const std::string&,
                              Action*) = 0;
  virtual std::string widgetName(inlib::xml::tree&) = 0;
  virtual void showShells(inlib::xml::tree&);
  virtual bool receiveScene(const std::string&,
                            const std::string&,
                            const std::string&) = 0;
public: //INTERNAL : used in tks destroy cbks.
  static bool is_in(const inlib::xml::tree&,inlib::xml::tree*);
protected:
  static inlib::xml::tree* findFirstWidget(inlib::xml::tree&);
private:
  bool load(inlib::xml::tree*,const std::string&,bool is_file,bool show,bool exec_create,inlib::xml::tree*&);
protected:
  bool destroyItemML(inlib::xml::tree*&);
  inlib::xml::tree* findItemML(const std::string&);
  void* findNativeWidget(const std::string&);
  inlib::xml::tree* createItemML(const std::string&,
                            const std::string&,
                            inlib::xml::tree*);
  std::string buildName(inlib::xml::tree&);
  inlib::xml::tree* parentItemML(inlib::xml::tree&);
  bool searchChild(inlib::xml::tree&,
                           const std::string&,
                           const std::string&,
                           std::vector<std::string>&) const;
  bool searchParent(inlib::xml::tree&,
                           const std::string&,
                           const std::string&,
                           std::vector<std::string>&) const;

  void removeDispatcher(Slash::UI::IUI::Dispatcher);
  bool toWidgetAndResource(const std::string&,std::string&,std::string&);
  bool element_value(const std::string&,const std::string&,std::string&);
  bool createIf(inlib::xml::tree&);
  bool createChildren(inlib::xml::tree&);
  void removeCyclics();
public:
  void removeInRelations(inlib::xml::tree&);
  bool netSteer();
protected: 
  //OnX::Action management :
  void addAction(Action*);
  void removeActions(OnX::Action::NativeWidget);
  std::vector<std::string> disableCallbackScript(OnX::Action::NativeWidget,const std::string&);
  bool enableCallbackScript(OnX::Action::NativeWidget,const std::string&,const std::vector<std::string>&);
private:
  Action* getAction(OnX::Action::NativeWidget,const std::string&,int = 0);
private:
  static std::string subTree(inlib::xml::tree&,int);
private:
  bool setSessionParameters(inlib::xml::tree&);
  bool setAliases(inlib::xml::tree&);
  bool solveInputDialog(inlib::xml::tree&);
  bool solveCopy(inlib::xml::tree*&);
  bool createInputDialog(inlib::xml::tree&);
  bool createNativeWidget(inlib::xml::tree&);
  void createWidgetTable();
  void createLanguageList();
  void createPropertyList();
protected:
  std::list<Action*> fActions;
  Slash::Core::ISession& fSession;
  std::string fName;
  CallbackData fCallbackData;
  std::list<Slash::UI::IUI::Dispatcher> fDispatchers;
  inlib::xml::tree* fTopItemML;
  Slash::UI::IScriptManager* fScriptManager;
  Slash::Net::INetManager* fNetManager;
  exlib::xml::file_getter* fFileGetter;
  Slash::UI::ICyclic* fNetCyclic;
  std::list<Slash::UI::ICyclic*> fCyclics;
  int fWidgetNameCounter;
  bool fBeingDestroyed;
  int fVerboseLevel;
  int fSteerLevel;
  Slash::Core::IRelationManager* fRelationManager;
  Slash::UI::IWidget* fCurrentWidget;
  bool fAskToTerminal;
private: //IB
  std::vector<Slash::UI::IWidgetClass*> fWidgetClasses;
  std::vector<std::string> fCallbackLanguageList;
  std::vector<std::string> fPropertyList;
  WidgetFactory fWidgetFactory;
  std::vector<Slash::UI::IWidgetFactory*> fFactories;
};

}

#endif


