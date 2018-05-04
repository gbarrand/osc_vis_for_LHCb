#ifndef OnX_QtUI_h
#define OnX_QtUI_h 

// Inheritance :
#include <OnX/Core/BaseUI.h>

#include <vector>
#include <list>

#include <QtCore/qglobal.h> //For QT_VERSION.

class QApplication;
class QWidget;
class QMessageBox;
class QObject;
class QEventLoop;

//namespace OnX {class Qt_ICallback;}
//class QtCallback;

class OnX_QMessageWidget;
class QListViewItem;

namespace OnX {class TermCyclic;}

namespace OnX {

class QtUI : public BaseUI {
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
  virtual Slash::UI::ICyclic* createCyclic(const std::string&,
                                int,const std::string&,const std::string&);
  virtual bool notify(Slash::Core::IMessage*);
  virtual bool enableTerminal(const std::string&,const std::string&);
  virtual bool disableTerminal();
  virtual std::string ask(const std::string&);
protected: //OnX::BaseUI
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
                              Action*);
  virtual std::string widgetName(inlib::xml::tree&);
  virtual bool receiveScene(const std::string&,
                            const std::string&,
                            const std::string&);
public:
  QtUI(Slash::Core::ISession&,const std::vector<std::string>&);
  virtual ~QtUI();
private:
  QtUI(const QtUI&);
  QtUI& operator=(const QtUI&);
public:
  QApplication* getQApplication() const;
  void removeQWidget(QWidget*);
private:
  static bool createTree(QWidget*,inlib::xml::tree*);
  static std::string getType(const std::string&);
  static std::string getCallbackName(const QObject&,const std::string&);
  static std::string getXMLTree(QListViewItem*);
private:
  bool destroyOneWidget(inlib::xml::tree&);
  void addCallbacks(inlib::xml::tree&,QWidget*,
                    const std::string&,
                    const std::string&,
                    const std::string& = "");
protected:
  QApplication* fQApplication;
private:
  bool fQApplication_owner;
  // Console panel :
  QWidget* fConsoleText;
  QMessageBox* fWarningDialog;
  //std::vector<OnX::Qt_ICallback*> fQtCallbacks;
  OnX_QMessageWidget* fMessageWidget;
  std::list<QWidget*> fWidgets;
  std::list<QEventLoop*> fEventLoopStack;
  TermCyclic* fTermCyclic;
};

}

#endif

