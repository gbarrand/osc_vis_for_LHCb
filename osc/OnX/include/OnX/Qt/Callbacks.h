#ifndef OnX_Qt_Callbacks_h
#define OnX_Qt_Callbacks_h 

#include <QtCore/qobject.h>
#include <OnX/Core/Action.h>

namespace inlib {namespace xml{class tree;}}
namespace OnX {class BaseUI;}

class QCallbackData;

class QTreeWidgetItem;

class QtCallback : public QObject, public OnX::Action {
  Q_OBJECT
public:
  QtCallback(Slash::UI::IUI&,
             Slash::UI::IWidget*,
             OnX::Action::NativeWidget,
             const std::string&,const std::string&,
             const OnX::Action::Parameters&,
             const std::string&);
  //FIXME : cp cstor.
  virtual ~QtCallback();
private:
  QtCallback(const QtCallback&);
  QtCallback& operator=(const QtCallback&);
public slots:
  void execute();
  void execute2(QCallbackData&);
  void execute3(QTreeWidgetItem*);
};

namespace OnX {
class CloseCallback : public QObject, public OnX::Action {
  Q_OBJECT
public:
  CloseCallback(BaseUI&,inlib::xml::tree*);
  virtual ~CloseCallback();
private:
  CloseCallback(const CloseCallback&);
  CloseCallback& operator=(const CloseCallback&);
public slots:
  void execute();
private:
  BaseUI& fBaseUI;
  inlib::xml::tree* fItemML;
};
}

/* NOTE : not needed.
namespace OnX {
class DestroyCallback : public QObject, public OnX::Action {
  Q_OBJECT
public:
  DestroyCallback(BaseUI&,inlib::xml::tree*,QWidget*);
  virtual ~DestroyCallback();
public slots:
  void execute();
private:
  inlib::xml::tree* fItemML;
  QWidget* fWidget;
};
}
*/

#endif
