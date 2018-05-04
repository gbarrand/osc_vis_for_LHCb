#ifndef OnX_GtkUI_h
#define OnX_GtkUI_h 

// Inheritance :
#include <OnX/Core/BaseUI.h>

#include <gtk/gtk.h>
#include <OnX/Gtk/gtkpageviewer.h> //FIXME

namespace OnX {class TermCyclic;}
namespace OnX {namespace Gtk{class Destroy;}}

namespace OnX {

class GtkUI : public BaseUI {
public: //Slash::UI::IUI
  virtual void* cast(const std::string&) const;
  virtual void put(const std::string&);
  virtual void synchronize();
  virtual void exit(int = 0);
  virtual bool lock();
  virtual bool unlock();
  virtual void warn(const std::string&);
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&);
  virtual void deleteChildren(const std::string&);
  virtual Slash::UI::ICyclic* createCyclic(const std::string&,
                                int,const std::string&,const std::string&);
  virtual bool notify(Slash::Core::IMessage*);
  virtual bool enableTerminal(const std::string&,const std::string&);
  virtual bool disableTerminal();
public: //OnX::BaseUI
  virtual int nativeSteer();
  virtual bool createNativeWidget(inlib::xml::tree&,
                            const std::string&,
                            const std::string&,
                            inlib::xml::tree*);
  virtual bool destroyNativeWidget(inlib::xml::tree&);
  virtual Action* addCallback(inlib::xml::tree&,
                           const std::string&,
                           const std::string&,
                           const std::string&);
  virtual void deleteCallback(inlib::xml::tree&,
                              const std::string&,
                              OnX::Action*);
  virtual std::string widgetName(inlib::xml::tree&);
  virtual bool receiveScene(const std::string&,
                            const std::string&,
                            const std::string&);
  virtual std::string ask(const std::string&);
public:
  GtkUI(Slash::Core::ISession&,const std::vector<std::string>&);
  virtual ~GtkUI();
private:
  GtkUI(const GtkUI&);
  GtkUI& operator=(const GtkUI&);
private:
  static void callback(GtkObject*,gpointer);
  static void pageCallback(GtkObject*,GtkCallbackStruct*,gpointer);
  static void treeViewActivatedCallback(GtkTreeView*,GtkTreePath*,GtkTreeViewColumn*,gpointer);
  static void treeViewOpenCloseCallback(GtkTreeView*,GtkTreeIter*,GtkTreePath*,gpointer);
  static void notebookCallback(GtkObject*,GtkNotebookPage*,guint,gpointer);
  static void closeCallback(GtkObject*,GdkEventAny*,gpointer);
  static void destroyCallback(GtkObject*,gpointer);
  static void destroyShellCallback(GtkObject*,gpointer);
  static void destroyConsoleTextCallback(GtkObject*,gpointer);
  static void destroyWarningDialogCallback(GtkObject*,gpointer);
  static gboolean avoidDeleteCallback(GtkObject*,gpointer);
  //static void listCallback(GtkObject*,GdkEvent*,gpointer);
  static void exposeCallback(GtkObject*,GdkEventExpose*,gpointer);
  static void eventCallback(GtkObject*,GdkEvent*,gpointer);
  static void cancelColorDialogCallback(GtkObject*,gpointer);
  static gint timer_message(gpointer);

  static bool fillModel(GtkWidget*,GtkTreeStore *,GtkTreeIter*,inlib::xml::tree*);
private:
  static bool createTree(GtkWidget*,inlib::xml::tree*,std::ostream&);
  static int getCompound(const std::string&);
  static GtkType getType(const std::string&);
  static std::string getCallbackName(GtkType,const std::string&);
  static void setTreeSelection(GtkWidget*,const std::string&);
  static std::string getTreeSelection(GtkWidget*);
private:
  void addCallbacks(inlib::xml::tree&,GtkWidget*,const std::string&,GtkObject*,const std::string&,GtkSignalFunc = (GtkSignalFunc)OnX::GtkUI::callback);
private:
  GdkWindow* fTopWindow;
  GtkWidget* fConsoleText;
  GtkWidget* fWarningDialog;
  GdkFont* fFixedFont;
  GtkAccelGroup* fAccelGroup;
  std::list<GtkWidget*> fWidgets;
  std::vector<Gtk::Destroy*> fDestroys;
  int fExitCode;
  TermCyclic* fTermCyclic;
};

}

#endif

